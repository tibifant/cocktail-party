FROM amd64/ubuntu:noble

WORKDIR /app

RUN apt update
RUN apt install -y llvm-18 clang-18 build-essential
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100

COPY ../backend ./backend
COPY ../buildscripts ./buildscripts
COPY ../premake ./premake
COPY ../premake5.lua .

CMD ["/bin/bash"]