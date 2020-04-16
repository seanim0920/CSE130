#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int fdcat(int input_fd, int output_fd)
{
  char fixed_buffer[16384]; //16384 chars in 32 KiB
  ssize_t num_bytes;
  while ((num_bytes = read(input_fd, fixed_buffer, sizeof(fixed_buffer))) > 0)
    {
      if (write(output_fd, fixed_buffer, num_bytes) != num_bytes)
        return -1;
    }
  return (num_bytes < 0) ? -1 : 0;
}

void readFromSTD()
{
    if (fdcat(0, 1) != 0) //read from standard input
      fprintf(stderr, "failed to copy standard input\n");
}

int main(int argc, char* argv[])
{
  if (argc == 1) //if no arguments given
    {
      readFromSTD();
    }
  else
    {
      for (int i = argc-1; i >= 1; i--) //iterate through arguments backwards
        {
          if (argv[i][0] == '-')
            {
              readFromSTD();
            }
          else
            {
              int fd = open(argv[i], O_RDONLY);
              if (fd < 0)
                fprintf(stderr, "failed to open %s for reading\n", argv[i]);
              else
                {
                  if (fdcat(fd, 1) != 0)
                    fprintf(stderr, "failed to copy %d to standard output\n", argv[i]);
                  close(fd);
              }
            }
        }
    }
}
