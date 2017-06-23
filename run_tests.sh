#!/bin/sh

similarities="0.97 0.98 0.9825"

# test IMAGE NHOODS SIMILARITIES REDUCES
test() {
	for nhood in $2; do
		for similarity in $3; do
			for reduce in $4; do
				build/image_to_graph $1 $nhood $similarity $reduce
			done
		done
	done
}

test "assets/lena.jpg" "35 40 45" "$similarities" "2 3"
test "assets/hand1.jpg" "10 15 20" "$similarities" 4
test "assets/hand2.jpg" "10 15 20" "$similarities" 4
