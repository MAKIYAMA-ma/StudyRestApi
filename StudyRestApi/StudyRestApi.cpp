#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define BUF_SIZE 65536

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t totalSize = size * nmemb;
  char* buffer = (char*)userp;
  errno_t err = strncat_s(buffer, BUF_SIZE, (const char*)contents, totalSize);
  if (err != 0) {
    fprintf(stderr, "strncat_s failed: %d\n", err);
  }
  return totalSize;
}

int main(int argc, char *argv[]) {
  char command[16] = "posts";
  char url[64] = "https://jsonplaceholder.typicode.com/";

  if(argc > 1) {
    strncpy_s(command, sizeof(command), argv[1], sizeof(command));
  }
  printf("%s\n", command);
  errno_t err = strncat_s(url, 64, command, 64);

  CURL* curl = curl_easy_init();
  if (curl) {
    char buffer[BUF_SIZE] = {0};

    /* curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.110/index.html"); */
    /* curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/"); */
    /* curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts"); */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "../../cert/cacert.pem");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // 開発中のみ！ 本番ではNG！！
    /* curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); */
    /* curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); */

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
      printf("Received:\n%s\n", buffer);
    } else {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  }
  return 0;
}
