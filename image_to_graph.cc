#include <cstdlib>
#include <fstream>
#include <iostream>

// igraph includes
#include <igraph/igraph.h>

// opencv includes
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// pixel neighborhood size
#define NSPAN 30

int main(int argc, char *argv[])
{
    // Failure: TODO
    if (argc < 2)
        return EXIT_FAILURE;

    // Step 1: TODO
    std::cerr << "Reading image... ";

    cv::Mat image;  // image we'll be processing
    image = cv::imread(argv[1], cv::IMREAD_REDUCED_GRAYSCALE_4);

    // Failure: TODO
    if (image.empty()) {
        std::cerr << "Failure on image read." << std::endl;
        return EXIT_FAILURE;
    }

    std::cerr << "Done. Image is " << image.cols << " by " << image.rows << "." << std::endl;

    // Step 2.1: TODO
    std::cerr << "Building graph... ";

    igraph_t graph;
    igraph_integer_t graph_dims = image.rows * image.cols;

    // Failure: TODO
    if (igraph_empty(&graph, graph_dims, IGRAPH_UNDIRECTED)) {
        std::cerr << "Failure on graph build." << std::endl;
        return EXIT_FAILURE;
    }

    //#pragma omp parallel for collapse(2) schedule(static) shared(image,graph)
    for (int irow = 0; irow < image.rows; irow++){
        for (int icol = 0; icol < image.cols; icol++) {

            //std::cerr << "  Doing pixel " << irow*image.rows+icol << "." << std::endl;

            int irow_n_min = std::max(0, irow-NSPAN);
            int icol_n_min = std::max(0, icol-NSPAN);
            int irow_n_max = std::min(image.rows, irow+NSPAN);
            int icol_n_max = std::min(image.cols, icol+NSPAN);

            for (int irow_n = irow_n_min; irow_n < irow_n_max; irow_n++) {
                for (int icol_n = icol_n_min; icol_n < icol_n_max; icol_n++) {

                    if (irow_n == irow && icol_n == icol)
                        continue;

                    //#pragma omp critical
                    {
                        if (255-std::abs(image.at<int>(irow,icol)-image.at<int>(irow_n,icol_n)) >= 126)
                            igraph_add_edge(&graph, irow*image.rows+icol, irow_n*image.rows+icol_n);
                    }
                }
            }
        }
    }

    std::cerr << "Done. Graph has " << igraph_ecount(&graph) << " edges." << std::endl;

    // Step 2.2: TODO
    std::cerr << "Writing to file... ";

    {
        using std::ofstream;
        ofstream graph_file;  // output graph to this file

        graph_file.open("image_as_graph.txt", ofstream::out|ofstream::trunc);

        for(igraph_integer_t iedge = 0; iedge < igraph_ecount(&graph); iedge++) {
            igraph_integer_t node1;  // connected node 1
            igraph_integer_t node2;  // connected node 2
            igraph_edge(&graph, iedge, &node1, &node2);
            graph_file << node1 << " " << node2 << std::endl;
        }

        graph_file.close();
    }

    std::cerr << "Done." << std::endl;

    // Step 3: TODO
    cv::namedWindow("Main Window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Main Window", image);
    cv::waitKey(0);

    // Step 4: TODO
    igraph_destroy(&graph);

    return EXIT_SUCCESS;
}
