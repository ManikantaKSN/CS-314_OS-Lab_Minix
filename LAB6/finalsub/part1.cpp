#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>
using namespace std;

struct Pixel {
    int r,g,b;
};

void grayScale_T1(vector<vector<Pixel>> &img) {
    //luminosity method . source: https://www.jeremymorgan.com/tutorials/opencv/how-to-grayscale-image/
    int height = img.size();
    int width = img[0].size();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double avg = 0.299 * img[i][j].r + 0.587 * img[i][j].g + 0.114 * img[i][j].b;
            img[i][j].r = (int)avg;
            img[i][j].g = (int)avg;
            img[i][j].b = (int)avg;
        }
    }
}

void edgeDetection_T2(vector<vector<Pixel>> &img) {
    //source : https://en.wikipedia.org/wiki/Edge_detection
    int width = img[0].size();
    int height = img.size();
    vector<vector<Pixel>> modifiedimg = img;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            modifiedimg[i][j] = {0,0,0};
        }
    }

    for (int i = 1; i <= height - 2; i++) {
        for (int j = 1; j <= width - 2; j++) {
            modifiedimg[i][j].r = sqrt(pow((img[i - 1][j - 1].r + 2 * img[i][j - 1].r + img[i + 1][j - 1].r) - (img[i - 1][j + 1].r + 2 * img[i][j + 1].r + img[i + 1][j + 1].r), 2) + pow((img[i - 1][j - 1].r + 2 * img[i - 1][j].r + img[i - 1][j + 1].r) - (img[i + 1][j - 1].r + 2 * img[i + 1][j].r + img[i + 1][j + 1].r), 2));
            modifiedimg[i][j].g = sqrt(pow((img[i - 1][j - 1].g + 2 * img[i][j - 1].g + img[i + 1][j - 1].g) - (img[i - 1][j + 1].g + 2 * img[i][j + 1].g + img[i + 1][j + 1].g), 2) + pow((img[i - 1][j - 1].g + 2 * img[i - 1][j].g + img[i - 1][j + 1].g) - (img[i + 1][j - 1].g + 2 * img[i + 1][j].g + img[i + 1][j + 1].g), 2));
            modifiedimg[i][j].b = sqrt(pow((img[i - 1][j - 1].b + 2 * img[i][j - 1].b + img[i + 1][j - 1].b) - (img[i - 1][j + 1].b + 2 * img[i][j + 1].b + img[i + 1][j + 1].b), 2) + pow((img[i - 1][j - 1].b + 2 * img[i - 1][j].b + img[i - 1][j + 1].b) - (img[i + 1][j - 1].b + 2 * img[i + 1][j].b + img[i + 1][j + 1].b), 2));
        }
    }
    img = modifiedimg;
}

void imageBlur_T3(vector<vector<Pixel>> &img) {
    //source : https://stackoverflow.com/questions/58926559/blurring-an-image-with-c
    int width = img[0].size();
    int height = img.size();
    vector<vector<Pixel>> modifiedimg = img;
    int cnt, sR, sG, sB;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cnt = sR = sG = sB = 0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    if (i + k > height-1 || i + k < 0 || j + l > width-1 || j + l < 0) continue;
                    sR += img[i + k][j + l].r;
                    sG += img[i + k][j + l].g;
                    sB += img[i + k][j + l].b;
                    cnt++;
                }
            }
            modifiedimg[i][j].r = round((double)sR / cnt);
            modifiedimg[i][j].g = round((double)sG / cnt);
            modifiedimg[i][j].b = round((double)sB / cnt);
        }
    }
    img = modifiedimg;
}

int main(int argc, char *argv[]){
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        exit(1);
    }

    ifstream fl(argv[1], ios::binary);
    if (!fl.is_open()) {
        cout << "Error: Could not open input file\n";
        exit(1);
    }
    string fmt, line;
    getline(fl, fmt);
    getline(fl, line);
    while (line[0] == '#') {
        getline(fl, line);
    }
    int columns, rows, max;
    stringstream ss(line);
    ss >> columns >> rows;
    getline(fl, line);
    max = stoi(line);
    vector<vector<Pixel>> image(rows, vector<Pixel>(columns)); //2D array to hold pixel data from input
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            int r, g, b;
            fl >> r >> g >> b;
            image[i][j] =  {r,g,b};
        }
    }
    fl.close();
    
    grayScale_T1(image);
    edgeDetection_T2(image);
    //imageBlur_T3(image);

    FILE *fp = fopen(argv[2], "w");
	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", columns, rows);
	fprintf(fp, "%d\n", max);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
            fprintf(fp, "%d %d %d ", image[i][j].r, image[i][j].g, image[i][j].b);
		}
		fprintf(fp, "\n");
	}
    fclose(fp);
    return 0;
}
