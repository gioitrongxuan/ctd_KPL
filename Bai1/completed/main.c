#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_STOPWORDS 1000
#define MAX_LINES 1000
#define MAX_WORDS 10000

typedef struct {
    char word[MAX_WORD_LEN];
    int lines[MAX_LINES];
    int count;
} WordIndex;

WordIndex index_list[MAX_WORDS];
int total_words = 0;

char stopword_list[MAX_STOPWORDS][MAX_WORD_LEN];
int total_stopwords = 0;

// Chuyển chuỗi sang chữ thường
void convert_to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

// Kiểm tra xem từ có phải stopword hay không
int is_in_stopwords(const char *word) {
    for (int i = 0; i < total_stopwords; i++) {
        if (strcmp(word, stopword_list[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Kiểm tra từ có chứa số hay không
int has_digit(const char *word) {
    while (*word) {
        if (isdigit(*word)) {
            return 1;
        }
        word++;
    }
    return 0;
}

// Phân tích từ từ một dòng văn bản
void extract_words(const char *line, char words[][MAX_WORD_LEN], int *word_count) {
    *word_count = 0;
    const char *start = line;
    char word[MAX_WORD_LEN];
    int word_length = 0;

    while (*start) {
        if (isalnum(*start)) {
            word[word_length++] = *start;
        } else if (word_length > 0) {
            word[word_length] = '\0';
            strcpy(words[(*word_count)++], word);
            word_length = 0;
        }
        start++;
    }

    // Xử lý từ cuối dòng nếu có
    if (word_length > 0) {
        word[word_length] = '\0';
        strcpy(words[(*word_count)++], word);
    }
}

// Thêm từ vào bảng chỉ mục
void add_word_to_index(const char *word, int line_number) {
    for (int i = 0; i < total_words; i++) {
        if (strcmp(index_list[i].word, word) == 0) {
            if (index_list[i].count == 0 || index_list[i].lines[index_list[i].count - 1] != line_number) {
                index_list[i].lines[index_list[i].count++] = line_number;
            }
            return;
        }
    }

    // Thêm từ mới vào chỉ mục
    strcpy(index_list[total_words].word, word);
    index_list[total_words].lines[0] = line_number;
    index_list[total_words].count = 1;
    total_words++;
}

// Sắp xếp các mục chỉ mục theo thứ tự từ điển
int compare_words(const void *a, const void *b) {
    return strcmp(((WordIndex *)a)->word, ((WordIndex *)b)->word);
}

int main() {
    FILE *text_file = fopen("vanban.txt", "r");
    FILE *stopword_file = fopen("stopw.txt", "r");

    if (!text_file || !stopword_file) {
        printf("Không thể mở tệp!\n");
        return 1;
    }

    // Đọc danh sách stopword
    while (fgets(stopword_list[total_stopwords], MAX_WORD_LEN, stopword_file)) {
        stopword_list[total_stopwords][strcspn(stopword_list[total_stopwords], "\n")] = '\0';
        convert_to_lowercase(stopword_list[total_stopwords]);
        total_stopwords++;
    }

    char line[1000];
    int line_number = 0;

    // Đọc tệp văn bản và xử lý từng dòng
    while (fgets(line, sizeof(line), text_file)) {
        line_number++;
        char words[MAX_WORDS][MAX_WORD_LEN];
        int word_count = 0;

        extract_words(line, words, &word_count);

        for (int i = 0; i < word_count; i++) {
            convert_to_lowercase(words[i]);

            if (!is_in_stopwords(words[i]) && !has_digit(words[i])) {
                add_word_to_index(words[i], line_number);
            }
        }
    }

    // Sắp xếp danh sách chỉ mục
    qsort(index_list, total_words, sizeof(WordIndex), compare_words);

    // In kết quả
    for (int i = 0; i < total_words; i++) {
        printf("%s ", index_list[i].word);
        for (int j = 0; j < index_list[i].count; j++) {
            printf("%d", index_list[i].lines[j]);
            if (j < index_list[i].count - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    fclose(text_file);
    fclose(stopword_file);
    return 0;
}
