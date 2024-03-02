#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
using namespace std;

struct Pixel {
    int r,g,b;
};

void grayScale_T1(key_t key1, key_t smkey1, int c, int r) {
    //luminosity method . source: https://www.jeremymorgan.com/tutorials/opencv/how-to-grayscale-image/
    
    int shmid_img = shmget(key1, sizeof(struct Pixel) * r * c, IPC_CREAT | 0666);
    if (shmid_img < 0) {
        perror("shmget error"); // if shared memory is not created
        exit(1);
    }
    struct Pixel *image = (struct Pixel *)shmat(shmid_img, NULL, 0); //2D array to hold pixel data from input
    
    int sid = shmget(smkey1, sizeof(sem_t), IPC_CREAT | 0666);
    if (sid < 0) {
        perror("shmget error"); // if shared memory is not created
        exit(1);
    }
    sem_t *sm = (sem_t *)shmat(sid, NULL, 0);

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            sem_wait(sm);
            double avg = 0.299 * image[i * c + j].r + 0.587 * image[i * c + j].g + 0.114 * image[i * c + j].b;
            image[i * c + j].r = (int)avg;
            image[i * c + j].g = (int)avg;
            image[i * c + j].b = (int)avg;
            sem_post(sm);
        }
    }
}

void edgeDetection_T2(key_t key1, key_t smkey1, int c, int r) {
    //source : https://en.wikipedia.org/wiki/Edge_detection

    int shmid_img = shmget(key1, sizeof(struct Pixel) * r * c, IPC_CREAT | 0666);
    if (shmid_img < 0) {
        perror("shmget error"); // if shared memory is not created
        exit(1);
    }
    struct Pixel *image = (struct Pixel *)shmat(shmid_img, NULL, 0); //2D array to hold pixel data from input
    
    int sid = shmget(smkey1, sizeof(sem_t), IPC_CREAT | 0666);
    if (sid < 0) {
        perror("shmget error"); // if shared memory is not created
        exit(1);
    }
    sem_t *sm = (sem_t *)shmat(sid, NULL, 0);

    vector<vector<Pixel>> modifiedimg(r, vector<Pixel>(c));
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            modifiedimg[i][j].r = modifiedimg[i][j].g = modifiedimg[i][j].b = 0;
        }
    }
    for (int i = 1; i <= r - 2; i++) {
        for (int j = 1; j <= c - 2; j++) {
            modifiedimg[i][j].r = sqrt(pow((image[(i - 1)*c + j - 1].r + 2 * image[i*c + j - 1].r + image[(i + 1)*c + j - 1].r) - (image[(i - 1)*c + j + 1].r + 2 * image[i*c + j + 1].r + image[(i + 1)*c + j + 1].r), 2) + pow((image[(i - 1)*c + j - 1].r + 2 * image[(i - 1)*c + j].r + image[(i - 1)*c + j + 1].r) - (image[(i + 1)*c + j - 1].r + 2 * image[(i + 1)*c + j].r + image[(i + 1)*c + j + 1].r), 2));
            modifiedimg[i][j].g = sqrt(pow((image[(i - 1)*c + j - 1].g + 2 * image[i*c + j - 1].g + image[(i + 1)*c + j - 1].g) - (image[(i - 1)*c + j + 1].g + 2 * image[i*c + j + 1].g + image[(i + 1)*c + j + 1].g), 2) + pow((image[(i - 1)*c + j - 1].g + 2 * image[(i - 1)*c + j].g + image[(i - 1)*c + j + 1].g) - (image[(i + 1)*c + j - 1].g + 2 * image[(i + 1)*c + j].g + image[(i + 1)*c + j + 1].g), 2));
            modifiedimg[i][j].b = sqrt(pow((image[(i - 1)*c + j - 1].b + 2 * image[i*c + j - 1].b + image[(i + 1)*c + j - 1].b) - (image[(i - 1)*c + j + 1].b + 2 * image[i*c + j + 1].b + image[(i + 1)*c + j + 1].b), 2) + pow((image[(i - 1)*c + j - 1].b + 2 * image[(i - 1)*c + j].b + image[(i - 1)*c + j + 1].b) - (image[(i + 1)*c + j - 1].b + 2 * image[(i + 1)*c + j].b + image[(i + 1)*c + j + 1].b), 2));
        }
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            sem_wait(sm);
            image[i * c + j].r = modifiedimg[i][j].r;
            image[i * c + j].g = modifiedimg[i][j].g;
            image[i * c + j].b = modifiedimg[i][j].b;
            sem_post(sm);
        }
    }
}

void imageBlur_T3(key_t key1, key_t smkey1, int c, int r) {
    //source : https://stackoverflow.com/questions/58926559/blurring-an-image-with-c
    
    int shmid_img = shmget(key1, sizeof(struct Pixel) * r * c, IPC_CREAT | 0666);
    if (shmid_img < 0) {
        perror("shmget error"); // if shared memory is not created
        exit(1);
    }
    struct Pixel *image = (struct Pixel *)shmat(shmid_img, NULL, 0); //2D array to hold pixel data from input
    
    int sid = shmget(smkey1, sizeof(sem_t), IPC_CREAT | 0666);
    if (sid < 0) {
        perror("shmget error"); // if shared memory is not created
        exit(1);
    }
    sem_t *sm = (sem_t *)shmat(sid, NULL, 0);

    vector<vector<Pixel>> modifiedimg(r,vector<Pixel>(c));
    int cnt, sR, sG, sB;

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cnt = sR = sG = sB = 0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    if (i + k > r-1 || i + k < 0 || j + l > c-1 || j + l < 0) continue;
                    sR += image[(i + k)*c + j + l].r;
                    sG += image[(i + k)*c + j + l].g;
                    sB += image[(i + k)*c + j + l].b;
                    cnt++;
                }
            }
            modifiedimg[i][j].r = round((double)sR / cnt);
            modifiedimg[i][j].g = round((double)sG / cnt);
            modifiedimg[i][j].b = round((double)sB / cnt);
        }
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            sem_wait(sm);
            image[i * c + j].r = modifiedimg[i][j].r;
            image[i * c + j].g = modifiedimg[i][j].g;
            image[i * c + j].b = modifiedimg[i][j].b;
            sem_post(sm);
        
        }
    }
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

    key_t key = 0x1111;
    int shmid_img = shmget(key, sizeof(struct Pixel) * rows * columns, IPC_CREAT | 0666);
    if (shmid_img < 0) {
        perror("shmget error"); // if shared memory is not created
        exit(1);
    }
    struct Pixel *image = (struct Pixel *)shmat(shmid_img, NULL, 0); //2D array to hold pixel data from input
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            int r, g, b;
            fl >> r >> g >> b;
            image[i * columns + j] =  {r,g,b};
        }
    }
    fl.close();

    key_t smkey = 0x2222;
    int sid = shmget(smkey, sizeof(sem_t), IPC_CREAT | 0666);
    if (sid < 0) {
        perror("shmget error"); // if shared memory is not created
        exit(1);
    }
    sem_t *sm = (sem_t *)shmat(sid, NULL, 0);
    sem_init(sm, 0, 1);

    int pid = fork();
    if (pid == 0) { // child process
        edgeDetection_T2(key, smkey, columns, rows);
        exit(0);
    }
    else { // parent process
        grayScale_T1(key, smkey, columns, rows);
        wait(NULL);
    }

    FILE *fp = fopen(argv[2], "w");
	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", columns, rows);
	fprintf(fp, "%d\n", max);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
            fprintf(fp, "%d %d %d ", image[i * columns + j].r, image[i * columns + j].g, image[i * columns + j].b);
		}
		fprintf(fp, "\n");
	}
    shmctl(shmid_img, IPC_RMID, NULL); // removing shared memory
    shmctl(sid, IPC_RMID, NULL);
    fclose(fp);
    return 0;
}
