FROM bash:3.2
FROM gcc:9
ADD . /build
RUN ["./build.sh", "release"]
CMD ["./bin/ddos2", "--module mod_a", "--test-all"]

