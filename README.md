### Build

```sh
cd build
cmake ..
make
```

### Build and run using script

```sh
./buildnrun.sh
```

### Client usage

```sh
./client --image hello.png --resize 100x100 --brightness 100 --output new.png
```

### Install [OpenCV](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)

```sh
mkdir -p ~/repos && cd ~/repos
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
unzip opencv.zip
mkdir -p build && cd build
cmake ../opencv-4.x
cmake --build . -j$(nproc)
sudo make install
```

### Test frontend 
    
```sh
cd frontend
npm install
npm start
```

### Other dependencies
- gcc / g++ / clang
- make / cmake
- unzip
- wget
- npm