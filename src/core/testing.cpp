#include <filesystem>
#include <cmath>

#include "testing.h"

#ifndef ENABLE_SSE2
#define STBI_NO_SIMD
#endif

#include "stb_image.h"
#include "stb_image_write.h"

namespace fs = std::filesystem;
using namespace test;

GLFWwindow* window = nullptr;

const int TEST_SCREEN_WIDTH = 640;
const int TEST_SCREEN_HEIGHT = 480;
const char *OUTPUT_DIR = "test-results/";

double compareImages(const std::string &fname1, const std::string &fname2);

test::GraphicalTest::GraphicalTest() {
    glfwInit();
    window = glfwCreateWindow(TEST_SCREEN_WIDTH, TEST_SCREEN_HEIGHT, "Test Window", NULL, NULL);
}

test::GraphicalTest::~GraphicalTest() {
    glfwTerminate();
}

void test::GraphicalTest::assertScreenshot(const std::string& fname) {
    if (!fs::exists(OUTPUT_DIR)) {
        fs::create_directories(OUTPUT_DIR);
    }

    glfwMakeContextCurrent(window);

    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);

    // Allocate buffer to hold RGBA (4 components) data
    GLubyte *data = (GLubyte*)malloc(TEST_SCREEN_WIDTH * TEST_SCREEN_HEIGHT * 4);
    if (!data) return;

    // Read the pixels from the framebuffer
    glReadPixels(0, 0, TEST_SCREEN_WIDTH, TEST_SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Flip the image vertically because OpenGL scans from bottom to top
    GLubyte *flipped_data = (GLubyte*)malloc(TEST_SCREEN_WIDTH * TEST_SCREEN_HEIGHT * 4);
    if (!flipped_data) {
        free(data);
        return;
    }

    for (int y = 0; y < TEST_SCREEN_HEIGHT; y++) {
        memcpy(flipped_data + (TEST_SCREEN_HEIGHT - 1 - y) * TEST_SCREEN_WIDTH * 4, data + y * TEST_SCREEN_WIDTH * 4, TEST_SCREEN_WIDTH * 4);
    }

    // Write to file
    std::string path(OUTPUT_DIR + fname);
    if (!fs::exists(path)) {
        stbi_write_png(fname.c_str(), TEST_SCREEN_WIDTH, TEST_SCREEN_HEIGHT, 4, flipped_data, TEST_SCREEN_WIDTH * 4);
    }
    else {
        std::string tmpPath(fs::temp_directory_path().append(fname));
        stbi_write_png(tmpPath.c_str(), TEST_SCREEN_WIDTH, TEST_SCREEN_HEIGHT, 4, flipped_data, TEST_SCREEN_WIDTH * 4);

        try {
            ASSERT_LE(compareImages(path, tmpPath), 0.1f);
            fs::remove(tmpPath);
        }
        catch (const std::exception &ex) {
            fs::remove(tmpPath);
            throw;
        }
    }

    // Free the allocated memory
    free(data);
    free(flipped_data);
}

void convertToGrayscale(unsigned char* data, int width, int height, int channels) {
    if (channels < 3) {
        std::cerr << "Image must have at least 3 channels (RGB) for conversion." << std::endl;
        return;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int i = y * width * channels + x * channels;
            unsigned char gray = (unsigned char)(0.299 * data[i] + 0.587 * data[i + 1] + 0.114 * data[i + 2]);
            // Set R, G, B to grayscale value
            data[i] = data[i + 1] = data[i + 2] = gray;
        }
    }
}

// Function to compare two grayscale images
double compareImages(const unsigned char* imgData1, const unsigned char* imgData2, int width, int height, int channels) {
    if (channels < 1) {
        std::cerr << "Image channels must be at least 1." << std::endl;
        return -1.0;
    }

    int numDifferentPixels = 0;
    int totalPixels = width * height;

    for (int i = 0; i < totalPixels * channels; i += channels) {
        // Compare the first channel assuming grayscale value is replicated across all channels
        if (imgData1[i] != imgData2[i]) {
            numDifferentPixels++;
        }
    }

    double percentageChanged = (double) numDifferentPixels / totalPixels * 100.0;
    return percentageChanged;
}

double compareImages(const std::string &fname1, const std::string &fname2) {
    int width, height, channels;
    unsigned char* imgData1 = stbi_load("path_to_first_image.jpg", &width, &height, &channels, 0);
    unsigned char* imgData2 = stbi_load("path_to_second_image.jpg", &width, &height, &channels, 0);

    if (!imgData1 || !imgData2) {
        std::cerr << "Error in loading images" << std::endl;
        return -1; // Error loading images
    }

    convertToGrayscale(imgData1, width, height, channels);
    convertToGrayscale(imgData2, width, height, channels);

    double res = compareImages(imgData1, imgData2, width, height, channels);
    stbi_image_free(imgData1);
    stbi_image_free(imgData2);

    return res;
}

GraphicalTestResults::GraphicalTestResults(const std::string& moduleName)
    : moduleName(moduleName) {
}

GraphicalTestResults::~GraphicalTestResults() {
    std::cout << this->moduleName << "Graphical Test Complete\n";
}

void GraphicalTestResults::addTest(const std::string& name){

}
