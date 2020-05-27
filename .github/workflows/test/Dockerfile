FROM bash:3.2
FROM gcc:9
ADD . /build
CMD ["./build.sh", "release"]
ENTRYPOINT ["./bin/ddos2", "--module mod_a", "--test-all"]
