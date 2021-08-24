#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

// Specified operations
#define FUR_RENAME 1
#define FUR_DELETE 2 
#define FUR_DOWNLOAD 3

// This is the data type for the inputs
struct InputData {
    char *old_path;
    char *new_path;
    char *url;
    char *download_path;
};

static size_t write_data(void *ptr, size_t size, size_t num_members, void *stream) {
    size_t written = fwrite(ptr, size, num_members, (FILE *) stream);
    return written;
}

// Simply downloads stuff from a url to a speified name
int download_file(char *url, char *file_name) {
    CURL *curl_handle; 
    FILE *filehandle;
 
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Initialize this curl session
    curl_handle = curl_easy_init();

    // set the link to download 
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    // Don't show any info 
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);

    // Disable progress meter
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

    // Send all data to the write_data() function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    // This is to handle the http 3xx redirects
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    // open the file
    filehandle = fopen(file_name, "wb");
    if (filehandle) {
        // Write the content to the file handle
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, filehandle);

        // Get the file
        curl_easy_perform(curl_handle);

        // close the file
        fclose(filehandle);
    }

    // Cleanup
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return EXIT_SUCCESS;
}

// Performs an operation specified by the operation macro
// and InputData
int operation(int operation, struct InputData data) {   
    if (operation == FUR_RENAME) { 
        return rename(data.old_path, data.new_path);

    } else if (operation == FUR_DOWNLOAD) {
        return download_file(data.url, data.download_path);
    
    } else {
        return EXIT_FAILURE;
    }
}

int main() {
    download_file("https://google.com", "index.html");
    struct InputData data;
    data.old_path = "index.html";
    data.new_path = "main.html";
    return operation(FUR_RENAME, data);
}