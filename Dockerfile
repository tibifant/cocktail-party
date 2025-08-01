FROM amd64/ubuntu:noble

RUN apt update
RUN apt install -y llvm-18 clang-18 build-essential
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100
RUN update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-18 100 && \
update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-18 100

WORKDIR /app

COPY backend ./backend
COPY buildscripts ./buildscripts
COPY premake ./premake
COPY premake5.lua .

CMD ["/bin/bash"]