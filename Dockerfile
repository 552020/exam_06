# Use the official Ubuntu base image
FROM ubuntu:latest

# Set non-interactive mode for APT to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# First unminimize the system
RUN yes | unminimize

# Then install our packages
RUN apt-get update && apt-get install -y \
    man-db \
    manpages \
    manpages-dev \
    manpages-posix \
    manpages-posix-dev \
    build-essential \
    vim \
    locales \
    && locale-gen en_US.UTF-8 \
    && update-locale LANG=en_US.UTF-8 \
    && rm -rf /var/lib/apt/lists/*

# Set default working directory
WORKDIR /root

# Default command is bash in interactive mode
CMD ["bash"]