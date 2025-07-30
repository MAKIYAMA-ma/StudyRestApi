#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define BUF_SIZE 65536
#define EN_CERTIFICATE_VALIDATION

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t totalSize = size * nmemb;
  char* buffer = (char*)userp;
  errno_t err = strncat_s(buffer, BUF_SIZE, (const char*)contents, totalSize);
  if (err != 0) {
    fprintf(stderr, "strncat_s failed: %d\n", err);
  }
  return totalSize;
}

static int getMethod(char *url) {
  int ret = -1;
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

#ifdef EN_CERTIFICATE_VALIDATION
#else
    // 開発中のみ！ 本番ではNG！！
    /* curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); */
    /* curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); */
#endif

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
      printf("Received:\n%s\n", buffer);
    } else {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    ret = 0;
  }
  return ret;
}

static int postMethod(char *url, char *body) {
  int ret = -1;
  CURL* curl = curl_easy_init();
  struct curl_slist *headers = NULL;

  if (curl) {
    char buffer[BUF_SIZE] = {0};

    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "../../cert/cacert.pem");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

#ifdef EN_CERTIFICATE_VALIDATION
#else
    // 開発中のみ！ 本番ではNG！！
    /* curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); */
    /* curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); */
#endif

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
      printf("Received:\n%s\n", buffer);
    } else {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    ret = 0;
  }
  return ret;
}

int main(int argc, char *argv[]) {
  char command[16] = "posts";
  char url[64] = "https://jsonplaceholder.typicode.com/";
  char postMsg[128] = "";
  char type = 0;

  if(argc > 1) {
    type = (char)atoi(argv[1]);
  }
  if(argc > 2) {
    strncpy_s(command, sizeof(command), argv[2], sizeof(command));
  }
  if(argc > 3) {
    strncpy_s(postMsg, sizeof(postMsg), argv[3], sizeof(postMsg));
  }

  errno_t err = strncat_s(url, 64, command, 64);

  if(type == 0) {
    getMethod(url);
  } else if(type == 1) {
    postMethod(url, postMsg);
  }

  return 0;
}
