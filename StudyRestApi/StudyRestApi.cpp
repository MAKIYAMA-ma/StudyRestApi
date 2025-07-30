#include <stdio.h>
#include <curl/curl.h>

int main(void) {
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.110/index.html");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    return 0;
}