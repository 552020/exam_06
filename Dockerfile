# Use the official Ubuntu base image
FROM ubuntu:latest

# Set non-interactive mode for APT to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install required tools
RUN apt-get update && apt-get install -y unminimize \
    man \
    manpages \
    manpages-dev \
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