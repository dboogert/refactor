FROM archlinux/base:latest

RUN pacman -Sy

RUN pacman -S --noconfirm extra/llvm

RUN pacman -S --noconfirm extra/clang
RUN pacman -S --noconfirm extra/cmake
RUN pacman -S --noconfirm core/make
RUN pacman -S --noconfirm community/ninja

RUN pacman -S --noconfirm core/grep
RUN pacman -S --noconfirm extra/vim