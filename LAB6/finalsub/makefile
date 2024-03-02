all : part1 part2_1a part2_1b part2_2 part2_3

part1: part1.cpp
	g++ -o part1 part1.cpp
	time ./part1 sample_pp3_5mb.ppm output_part1.ppm

part2_1a: part2_1a.cpp
	g++ -o part2_1a part2_1a.cpp
	time ./part2_1a sample_pp3_5mb.ppm output_part2_1a.ppm

part2_1b: part2_1b.cpp
	g++ -o part2_1b part2_1b.cpp
	time ./part2_1b sample_pp3_5mb.ppm output_part2_1b.ppm

part2_2: part2_2.cpp
	g++ -o part2_2 part2_2.cpp
	time ./part2_2 sample_pp3_5mb.ppm output_part2_2.ppm

part2_3: part2_3.cpp
	g++ -o part2_3 part2_3.cpp
	time ./part2_3 sample_pp3_5mb.ppm output_part2_3.ppm

clean:
	rm -f part1 part2_1a part2_1b part2_2 part2_3 output_part1.ppm output_part2_1a.ppm output_part2_1b.ppm output_part2_2.ppm output_part2_3.ppm