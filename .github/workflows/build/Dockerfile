FROM bash:3.2
FROM gcc:9
ADD . /build
CMD ["ln", "-s", "`which gcc`", "/usr/bin/gcc-9"]
CMD ["./build.sh", "all"]

