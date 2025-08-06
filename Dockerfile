FROM amd64/ubuntu:noble

RUN apt update
RUN apt install -y llvm-18 clang-18 build-essential
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100
RUN update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-18 100 && \
update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-18 100
RUN apt install -y libasio-dev

WORKDIR /app

COPY backend/src ./backend/src
COPY backend/3rdParty ./backend/3rdParty
COPY backend/project.lua ./backend/project.lua
COPY --chmod=755 buildscripts/release.sh ./buildscripts/release.sh
COPY --chmod=755 buildscripts/debug.sh ./buildscripts/debug.sh
COPY premake ./premake
COPY premake5.lua .

CMD ["/bin/bash"]