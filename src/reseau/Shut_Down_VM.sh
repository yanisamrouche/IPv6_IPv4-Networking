cd VM1
echo "shut down VM1"
vagrant halt && vagrant destroy
cd ..

cd VM3
echo "shut down VM3"
vagrant halt && vagrant destroy
cd ..

cd VM1-6
echo "shut down VM1-6"
vagrant halt && vagrant destroy
cd ..

cd VM2-6
echo "shut down VM2-6"
vagrant halt && vagrant destroy
cd ..

cd VM3-6
echo "shut down VM3-6"
vagrant halt && vagrant destroy
cd ..