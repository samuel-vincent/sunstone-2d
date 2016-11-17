
mkdir ./libs

cd ./libs

git clone https://github.com/glfw/glfw.git
git clone https://github.com/g-truc/glm.git

cd ..

mkdir ./build

cd ./build && cmake ..

cd ..
