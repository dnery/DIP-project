#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

// igraph includes
#include <igraph.h>

// opencv includes
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// TODO
// - Use colored images for graph generation (Done)
// - Implement superpixels procedure
// - Do a shitload of tests

/// Show program usage
void usage(const char *program_name)
{
    std::cerr << "Usage: " << program_name << " <image name> "
        "<neighbourhood radius> <pixel similarity constant> "
        "[reduce: 1..3 (default: 1)]" << std::endl;
}

int main(int argc, char *argv[])
{
    // Step 0: process CLI arguments
    std::cerr << "ArgsParse... ";

    char  *image_name;       // input: image file name
    int   nhood_radius;      // input: pixel area scan radius
    float similarity_const;  // input: pixel similarity threshold
    int   imread_flags;      // possible input: image downscaling factor
    int   arg_error = 0;     // arguments parse error flag

    if (argc < 4) {
        arg_error = 1;
    } else {

        // Required
        image_name = argv[1];
        nhood_radius = atoi(argv[2]);
        similarity_const = atof(argv[3]);

        // Parse downscaling factor, if provided
        int reduction_idx = argc > 4 ? atoi(argv[4]) : 1;
        if (nhood_radius && similarity_const && reduction_idx > 0 && reduction_idx <= 3) {
            const int imread_possible_flags[] = {
                cv::IMREAD_COLOR,
                cv::IMREAD_REDUCED_COLOR_2,
                cv::IMREAD_REDUCED_COLOR_4,
            };
            // Reduction = 1 means no downscaling at all
            imread_flags = imread_possible_flags[reduction_idx - 1];
        } else {
            arg_error = 1;
        }
    }

    // Failure: TODO
    if (arg_error) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    std::cerr << "Done." << std::endl;
    // Step 0: end

    // Step 1: retrieve the image
    std::cerr << "Reading image... ";

    // Image to be processed, possibly downscaled
    cv::Mat image = cv::imread(image_name, imread_flags);

    // Failure: TODO
    if (image.empty()) {
        std::cerr << "Failure on image read." << std::endl;
        return EXIT_FAILURE;
    }

    // Normalized copy
    cv::Mat image_f;  // image copy, normalized
    image.convertTo(image_f, CV_32F, 1.0f/255.0f);

    // Failure: TODO
    if (image_f.empty()) {
        std::cerr << "Failure on image convert." << std::endl;
        return EXIT_FAILURE;
    }

    std::cerr << "Done. Image is " << image.cols << " by " << image.rows << ", ";
    std::cerr << (image.channels() > 1 ? "colored." : "grayscale.") << std::endl;
    // Ste 1: end

    // Step 2.1: build graph from image
    std::cerr << "Building graph... ";

    igraph_t graph;           // graph object
    // Failure: TODO
    if (igraph_empty(&graph, image.rows*image.cols, IGRAPH_UNDIRECTED)) {
        std::cerr << "Failure on graph allocation." << std::endl;
        return EXIT_FAILURE;
    }

    igraph_vector_t edges;    // graph edge array
    // Failure: TODO
    if (igraph_vector_init(&edges, 0)) {
        std::cerr << "Failure on edge array allocation." << std::endl;
        return EXIT_FAILURE;
    }

    int nch = image_f.channels();
    // Relate pixels that satisfy the weight function W(i,j) = 1 − |Ii−Ij| ≥ t
    // -> Since our values are normalized, 1 is replaced with 255 (max intensity)
    //    and t is chosen to be 254 (arbitrary measure, has to be studied better)
    for (int irow = 0; irow < image_f.rows; irow++) {
        float *scanline = image_f.ptr<float>(irow);

        for (int icol = 0; icol < image_f.cols; icol++) {
            float bvalue = scanline[icol*nch+0];
            float gvalue = scanline[icol*nch+1];
            float rvalue = scanline[icol*nch+2];

            //std::cerr << "  Pixel " << irow*image_f.cols+icol << " has value " << int(value) << " and ";

            // Define neighborhood around the pixel
            int irow_n_min = std::max(0, irow-nhood_radius);
            int icol_n_min = std::max(0, icol-nhood_radius);
            int irow_n_max = std::min(image_f.rows, irow+nhood_radius);
            int icol_n_max = std::min(image_f.cols, icol+nhood_radius);

            //std::cerr << (irow_n_max-irow_n_min)*(icol_n_max-icol_n_min) << " neighbours." << std::endl;

            // Look for similar pixels in this neighboorhood
            for (int irow_n = irow_n_min; irow_n < irow_n_max; irow_n++) {
                float *neighbour_scanline = image_f.ptr<float>(irow_n);

                for (int icol_n = icol_n_min; icol_n < icol_n_max; icol_n++) {
                    float neighbour_bvalue = neighbour_scanline[icol_n*nch+0];
                    float neighbour_gvalue = neighbour_scanline[icol_n*nch+1];
                    float neighbour_rvalue = neighbour_scanline[icol_n*nch+2];

                    // distance function
                    float similarity = 1.0f - sqrtf((bvalue-neighbour_bvalue)*(bvalue-neighbour_bvalue)
                            +(gvalue-neighbour_gvalue)*(gvalue-neighbour_gvalue)
                            +(rvalue-neighbour_rvalue)*(rvalue-neighbour_rvalue))
                        /sqrtf(3.0f);

                    // If the pixels have similarity_const greater than a certain measure, link them
                    if (similarity >= similarity_const) {
                        igraph_vector_push_back(&edges, irow*image_f.cols+icol);
                        igraph_vector_push_back(&edges, irow_n*image_f.cols+icol_n);
                    }
                }
            }
        }
    }

    // Add edges as batch (gotta go fast)
    if (igraph_add_edges(&graph, &edges, NULL)) {
        std::cerr << "Failure adding edges to graph." << std::endl;
        std::cerr << "Are all of your vertex IDs valid?" << std::endl;
        std::cerr << "Does your edge vector have an odd length?" << std::endl;
        return EXIT_FAILURE;
    }

    if (igraph_simplify(&graph, 1, 1, NULL)) {
        std::cerr << "Failure simplifying graph." << std::endl;
        std::cerr << "You're probably out of memory." << std::endl;
        return EXIT_FAILURE;
    }

    std::cerr << "Done. Graph has " << igraph_ecount(&graph) << " edges." << std::endl;
    // Step 2.1: end

#if 0
    // Step 2.2: write graph to a file
    std::cerr << "Writing to file... ";
    {
        using std::ofstream;
        ofstream graph_file;  // output graph to this file

        graph_file.open("image_to_graph.out", ofstream::out|ofstream::trunc);

        // Downsampled image dims outputted as header
        graph_file << image.rows << " " << image.cols << std::endl;

        // Graph is outputted to file as an edgelist
        for (igraph_integer_t iedge = 0; iedge < igraph_ecount(&graph); iedge++) {
            igraph_integer_t node1;  // connected node 1
            igraph_integer_t node2;  // connected node 2
            igraph_edge(&graph, iedge, &node1, &node2);
            graph_file << node1 << " " << node2 << std::endl;
        }

        graph_file.close();
    }
    std::cerr << "Done." << std::endl;
    // Step 2.2: end
#endif

    // Step 2.3: evaluate communities and separate segments
    std::cerr << "Evaluating communities... ";

    igraph_matrix_t merges;      // Merge steps array
    igraph_matrix_init(&merges, 0, 0);

    igraph_vector_t modularity;  // Graph mudularity array
    igraph_vector_init(&modularity, 0);

    igraph_vector_t membership;  // Community membership arrays
    igraph_vector_init(&membership, 0);

    // Apply fastgreedy algorithm
    igraph_community_fastgreedy(&graph, /*weights*/ NULL, &merges, &modularity,
            /*membership vector*/ NULL);

    std::cerr << "Done. ";

    // Membership vector
    size_t imod = igraph_vector_which_max(&modularity);  // max modularity index
    igraph_community_to_membership(&merges, igraph_vcount(&graph), imod, &membership, 0);

    std::cerr << "Max modularity is " << VECTOR(modularity)[imod]
        << " with " << (int)igraph_vector_max(&membership)
        << " communities." << std::endl;

    // Paint segments
    std::cerr << "Painting segments... ";

    cv::Mat image_s;                                     // image copy, to be painted
    cv::cvtColor(image, image_s, cv::COLOR_BGR2GRAY);

    // Failure: TODO
    if (image_s.empty()) {
        std::cerr << "Failure on image color to gray (cvtColor)." << std::endl;
        return EXIT_FAILURE;
    }

    float max_seg_val = igraph_vector_max(&membership);  // max segment value, for normalization
    for (long ipixel = 0; ipixel < image_s.rows*image_s.cols; ipixel++)
        image_s.data[ipixel] = (uchar)(VECTOR(membership)[ipixel]*255.0f/max_seg_val);

    std::cerr << "Done." << std::endl;
    // Step 2.3: end

    // Step 3: show segmentation results
    cv::Mat colored_segments;
    cv::applyColorMap(image_s, colored_segments, cv::COLORMAP_JET);

    cv::namedWindow("Output", cv::WINDOW_NORMAL);
    cv::namedWindow("Original", cv::WINDOW_NORMAL);

    cv::resizeWindow("Output", image.cols*5, image.rows*5);
    cv::resizeWindow("Original", image_s.cols*5, image_s.rows*5);

    cv::imshow("Original", image);
    cv::imshow("Output", colored_segments);
    while((cv::waitKey() & 0xEFFFFF) != 27);
    // Step 3: end

    // Step 4: cleanup & goodbye
    std::cerr << "Cleaning up memory... ";

    // Community allocation stuff
    igraph_vector_destroy(&membership);
    igraph_vector_destroy(&modularity);
    igraph_matrix_destroy(&merges);

    // Graph structure stuff
    igraph_vector_destroy(&edges);
    igraph_destroy(&graph);

    // Images
    image_s.release();
    image_f.release();
    image.release();

    std::cerr << "Done." << std::endl;
    // Step 4: end

    return EXIT_SUCCESS;
}
