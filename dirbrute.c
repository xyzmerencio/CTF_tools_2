#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void brute(const char *url, const char *wordlist_file) {
    FILE *file = fopen(wordlist_file, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char word[256];
    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = '\0'; // Remove o caractere de nova linha

        char url_final[512];
        snprintf(url_final, sizeof(url_final), "%s/%s", url, word);

        CURL *curl = curl_easy_init();
        if (curl) {
            CURLcode res;

            curl_easy_setopt(curl, CURLOPT_URL, url_final);
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // Faz apenas uma requisição HEAD

            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                long response_code;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
                if (response_code != 404) {
                    printf("%s -- %ld\n", url_final, response_code);
                }
            } else {
                fprintf(stderr, "Erro na requisição: %s\n", curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <url> <wordlist>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *url = argv[1];
    const char *wordlist_file = argv[2];

    brute(url, wordlist_file);

    return EXIT_SUCCESS;
}
