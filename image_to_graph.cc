#include <cstdlib>
#include <fstream>
#include <iostream>

// igraph includes
#include <igraph/igraph.h>

// opencv includes
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// pixel neighborhood size
#define NSPAN 15

int main(int argc, char *argv[])
{
    // Failure: TODO
    if (argc < 2)
        return EXIT_FAILURE;

    // Step 1: retrieve the image
    std::cerr << "Reading image... ";

    // Image to be processed, downscaled by a factor of 4
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_REDUCED_GRAYSCALE_4);

    // Failure: TODO
    if (image.empty()) {
        std::cerr << "Failure on image read." << std::endl;
        return EXIT_FAILURE;
    }

    std::cerr << "Done. Image is " << image.cols << " by " << image.rows << "." << std::endl;

    // Step 2.1: build graph from image
    std::cerr << "Building graph... ";

    igraph_t graph;         // graph object
    // Failure: TODO
    if (igraph_empty(&graph, image.rows*image.cols, IGRAPH_UNDIRECTED)) {
        std::cerr << "Failure on graph allocation." << std::endl;
        return EXIT_FAILURE;
    }

    igraph_vector_t edges;  // graph edge array
    // Failure: TODO
    if (igraph_vector_init(&edges, 0)) {
        std::cerr << "Failure on edge array allocation." << std::endl;
        return EXIT_FAILURE;
    }

    // Relate pixels that satisfy the weight function W(i,j) = 1 − |Ii−Ij| ≥ t
    // -> Since our values are normalized, 1 is replaced with 255 (max intensity)
    //    and t is chosen to be 126 (arbitrary measure, has to be studied better)
    for (int irow = 0; irow < image.rows; irow++) {
        for (int icol = 0; icol < image.cols; icol++) {
            uchar value = image.data[irow*image.cols+icol];

            //std::cerr << "  Pixel " << irow*image.cols+icol << " has value " << int(value) << " and ";

            // Define neighborhood around the pixel
            int irow_n_min = std::max(0, irow-NSPAN);
            int icol_n_min = std::max(0, icol-NSPAN);
            int irow_n_max = std::min(image.rows, irow+NSPAN);
            int icol_n_max = std::min(image.cols, icol+NSPAN);

            //std::cerr << (irow_n_max-irow_n_min)*(icol_n_max-icol_n_min) << " neighbours." << std::endl;

            // Look for similar pixels in this neighboorhood
            for (int irow_n = irow_n_min; irow_n < irow_n_max; irow_n++) {
                for (int icol_n = icol_n_min; icol_n < icol_n_max; icol_n++) {
                    uchar neighbour_value = image.data[irow_n*image.cols+icol_n];

                    // If the pixels have similarity greater than a certain measure, link them
                    if (255-std::abs(value-neighbour_value) >= 245) {
                        igraph_vector_push_back(&edges, irow*image.cols+icol);
                        igraph_vector_push_back(&edges, irow_n*image.cols+icol_n);
                    }
                }
            }
        }
    }

    // Add edges as batch (gotta go fast)
    igraph_add_edges(&graph, &edges, NULL);

    // Free igraph vector data
    igraph_vector_destroy(&edges);

    std::cerr << "Done. Graph has " << igraph_ecount(&graph) << " edges." << std::endl;

    // Step 2.2: write graph to a file
    std::cerr << "Writing to file... ";
    {
        using std::ofstream;
        ofstream graph_file;  // output graph to this file

        graph_file.open("image_as_graph.txt", ofstream::out|ofstream::trunc);

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

    // Step 3: show stuff and whatnot
    cv::namedWindow("Main Window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Main Window", image);
    cv::waitKey(0);

    // Step 4: cleanup & goodbye
    igraph_destroy(&graph);

    return EXIT_SUCCESS;
}
