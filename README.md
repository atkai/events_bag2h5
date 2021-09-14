# events_h52bag
C++ code to convert event data from HDF5 to ROSbags. By default, data format of h5 input file is expected as in the [DSEC dataset](https://dsec.ifi.uzh.ch/) or [TUM-VIE dataset](https://vision.in.tum.de/data/datasets/visual-inertial-event-dataset). The exact data structure used can be found [here](https://github.com/uzh-rpg/DSEC#events).

**Author**: [Suman Ghosh](https://www.linkedin.com/in/suman-ghosh-a8762576/)

# Installation
* Install ROS. We need the `rosbag` and `std_msgs` packages.
* Install libhdf5: 
```
sudo apt-get update
sudo apt-get install libhdf5-dev
```

* Install the [blosc filter plugin for hdf5](https://github.com/Blosc/hdf5-blosc#installing-the-blosc-filter-plugin) for read-only access across all HDF5 applications in your system:
```
git clone https://github.com/Blosc/hdf5-blosc.git
cd hdf5-blosc
mkdir build && cd build
cmake ..
make
```
* Copy the `libH5Zblosc.so` shared library generated inside the `build` folder into your local hdf5 plugin path, which is usually `/usr/local/hdf5/lib/plugin` or `/usr/lib/x86_64-linux-gnu/hdf5/plugins` depending on your Linux distro. Installation location of hdf5 can be found using `dpkg -L libhdf5-dev`.

* Add path to your installed `dvs_msgs` header in line 11 of `CMakeLists.txt`.  If not already installed, install `dvs_msgs` from https://github.com/uzh-rpg/rpg_dvs_ros in your catkin workspace.
* Clone this repository: `git clone https://github.com/tub-rip/events_h52bag.git`
* Compile this repository:
```
cd events_h52bag
mkdir build && cd build
cmake ..
make
```
# Execution
`./events_h52bag <path/to/input/h5file> <path/to/output/bagfile> <output_topic_name> <sensor_height> <sensor_width>`

# Working example
* Download input data from DSEC dataset: https://download.ifi.uzh.ch/rpg/DSEC/train/thun_00_a/thun_00_a_events_left.zip
* Extract `events.h5` from zip.
* Execute: `./events_h52bag events.h5 out.bag /dvs/left/events 480 640`

# Log
* 14.09.21: Sensor size can now be passed an argument. Set to 0 when no `t_offset` data is present in h5 file. Tested on TUM-VIE dataset.
