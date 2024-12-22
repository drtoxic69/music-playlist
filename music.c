#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char title[100];
    char artist[100];
    char filePath[256];
    struct Song* next;
    struct Song* prev;
} Song;

Song* head = NULL;
Song* current_song = NULL;

void playSong(Song* song) {
    if (song == NULL) return;

    char command[300];
    #ifdef _WIN32
        snprintf(command, sizeof(command), "start %s", song->filePath);
    #elif defined(__APPLE__)
        snprintf(command, sizeof(command), "open %s", song->filePath);
    #else
        snprintf(command, sizeof(command), "xdg-open %s", song->filePath);
    #endif
    system(command);
    printf("Now playing: %s by %s\n", song->title, song->artist);
}

Song* createSong(const char* title, const char* artist, const char* filePath) {
    Song* newSong = (Song*)malloc(sizeof(Song));
    strcpy(newSong->title, title);
    strcpy(newSong->artist, artist);
    strcpy(newSong->filePath, filePath);
    newSong->next = NULL;
    newSong->prev = NULL;
    return newSong;
}

void addSong(const char* title, const char* artist, const char* filePath) {
    Song* newSong = createSong(title, artist, filePath);

    if (head == NULL) {
        newSong->next = newSong->prev = newSong;
        head = current_song = newSong;
    } else {
        Song* last = head->prev;
        newSong->prev = last;
        last->next = newSong;
        newSong->next = head;
        head->prev = newSong;
    }
    printf("Added: %s by %s\n", title, artist);
}

void removeSong(const char* title) {
    if (head == NULL) {
        printf("Playlist is empty!\n");
        return;
    }

    Song* ptr = head;
    do {
        if (ptr->next == ptr && strcmp(ptr->title, title) == 0) {
            printf("Last song removed. Playlist is now empty!\n");
            free(ptr);
            head = current_song = NULL;
            return;
        }
        else if (strcmp(ptr->title, title) == 0) {
            Song* prev = ptr->prev;
            Song* next = ptr->next;
            prev->next = next;
            next->prev = prev;
            if (ptr == head) {
                head = next;
            }
            if (ptr == current_song) {
                current_song = next;
            }
            free(ptr);
            printf("Song removed: %s\n", title);
            return;
        }
        ptr = ptr->next;
    } while (ptr != head);

    printf("Song not found: %s\n", title);
}

void displayPlaylist() {
    if (head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }
    Song* temp = head;
    int index = 1;
    do {
        printf("%d. %s by %s%s\n",
            index++,
            temp->title,
            temp->artist,
            (temp == current_song) ? " (current)" : "");
        temp = temp->next;
    } while (temp != head);
}

void playNext() {
    if (current_song == NULL) {
        printf("No songs in playlist!\n");
        return;
    }
    current_song = current_song->next;
    playSong(current_song);
}

void playPrevious() {
    if (current_song == NULL) {
        printf("No songs in playlist!\n");
        return;
    }
    current_song = current_song->prev;
    playSong(current_song);
}

void playCurrent() {
    if (current_song == NULL) {
        printf("No songs in playlist!\n");
        return;
    }
    playSong(current_song);
}

void playSpecific(const char* title) {
    if (head == NULL) {
        printf("Playlist is empty!\n");
        return;
    }

    Song* ptr = head;
    do {
        if (strcmp(ptr->title, title) == 0) {
            current_song = ptr;
            playSong(ptr);
            return;
        }
        ptr = ptr->next;
    } while (ptr != head);

    printf("Song not found: %s\n", title);
}

void freePlaylist() {
    if (head == NULL) return;

    Song* current = head->next;
    while (current != head) {
        Song* temp = current;
        current = current->next;
        free(temp);
    }
    free(head);
    head = current_song = NULL;
}

int main() {
    int choice;
    char title[100], artist[100], filePath[256];

    while (1) {
        printf("\nMusic Player Menu:\n");
        printf("1. Add Song\n");
        printf("2. Remove Song\n");
        printf("3. Display Playlist\n");
        printf("4. Play Next Song\n");
        printf("5. Play Previous Song\n");
        printf("6. Play Current Song\n");
        printf("7. Play Specific Song\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter song title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;

                printf("Enter artist name: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = 0;

                printf("Enter file path: ");
                fgets(filePath, sizeof(filePath), stdin);
                filePath[strcspn(filePath, "\n")] = 0;

                addSong(title, artist, filePath);
                break;

            case 2:
                printf("Enter song title to remove: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                removeSong(title);
                break;

            case 3:
                displayPlaylist();
                break;

            case 4:
                playNext();
                break;

            case 5:
                playPrevious();
                break;

            case 6:
                playCurrent();
                break;

            case 7:
                printf("Enter song title to play: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                playSpecific(title);
                break;

            case 8:
                freePlaylist();
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
