#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

static size_t write_data(void *ptr, size_t size, size_t num_members, void *stream) {
    size_t written = fwrite(ptr, size, num_members, (FILE *) stream);
    return written;
}

int download_file(char *url, char *path) {
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
    filehandle = fopen(path, "wb");
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

int main() {
    return download_file("https://google.com", "index.html");
}