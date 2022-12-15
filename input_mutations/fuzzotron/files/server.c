#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#define PORT 1234

void echo_the_buffer(char *message, int sockfd)
{
  char echo_buffer[32];
  strcpy(echo_buffer, message);
  write(sockfd, echo_buffer, strlen(echo_buffer));
}

int main()
{
  char echo_buffer[32];
  char buffer[4096];

  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("Error opening socket");
    exit(1);
  }

  // Bind the socket to a local address
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT);
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
  signal(SIGPIPE, SIG_IGN);


  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("Error binding socket");
    exit(1);
  }

  // Listen for incoming connections
  listen(sockfd, 5);

  while (true)
  {
    // Accept an incoming connection
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
      perror("Error accepting connection");
      break;
    }

    memset(buffer, 0, sizeof(buffer));

    while (true)
    {
      int n = read(newsockfd, buffer, sizeof(buffer));
      if (n < 1)
      {
        perror("Error reading from socket");
        break;
      }

      // Parse the command and arguments from the received data
      char *cmd = strtok(buffer, " ");
      char *arg1 = strtok(NULL, " ");

      // Handle the received command
      if (strcmp(cmd, "VERSION\n") == 0)
      {
        char *version = "1.0.0\n";
        write(newsockfd, version, strlen(version));
      }
      else if (strcmp(cmd, "ECHO") == 0)
      {
        // Echo the received data back to the client
        echo_the_buffer(arg1, newsockfd);
      }
      else if (strcmp(cmd, "EXIT\n") == 0)
      {
        char *goodbye_msg = "Goodbye!\n";
        write(newsockfd, goodbye_msg, strlen(goodbye_msg));
        break;
      }
      else
      {
        // Unknown command
        char *error_msg = "Error: unknown command\n";
        write(newsockfd, error_msg, strlen(error_msg));
      }
    }

    // Close the connection
    printf("Client disconnected\n");
    close(newsockfd);
  }

  // Close the socket
  close(sockfd);

  return 0;
}