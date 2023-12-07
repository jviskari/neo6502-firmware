image=llvm-mos
docker run -d -it --name ${image} --mount type=bind,source=${PWD},target=/tmp/dev ${image}
docker exec -it ${image} /bin/sh
