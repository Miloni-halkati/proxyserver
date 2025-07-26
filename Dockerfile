# Use official Ubuntu base image
FROM ubuntu:22.04

# Set environment to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    wget \
    unzip \
    libpoco-dev \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files into container
COPY . /app

# Build the project
RUN mkdir build && cd build && cmake .. && make

# Expose the proxy port
EXPOSE 8080

# Run the proxy server
CMD ["./build/proxy"]
