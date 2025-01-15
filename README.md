# events_bag2h5
We add implementations to covert from ROSbags to HDF5, and keep the original C++ code to convert event data from HDF5 to ROSbags. More information about Installation and details about h52bag can be found in this repo 'https://github.com/tub-rip/events_h52bag'.


# Installation

* Do not forget to add path to your installed `dvs_msgs` header in line 11 of `CMakeLists.txt`. If not already installed, install `dvs_msgs` from https://github.com/uzh-rpg/rpg_dvs_ros in your catkin workspace.
* Clone this repository: `git clone https://github.com/atkai/events_bag2h5`
* Compile this repository:
```
cd events_bag2h5
mkdir build && cd build
cmake ..
make
```


# Execution
## ROSBag to HDF5
`./events_h52bag <path/to/input/bagfile> <path/to/output/h5file(without extension)> <event_topic_name> [optional, default="events"]<dataset_prefix>`



# Working example
* Download input data from DSEC dataset: https://rpg.ifi.uzh.ch/datasets/davis/boxes_6dof.bag
* Execute: `./events_bag2h5 boxes_6dof.bag out /dvs/events`
* The output ROSBag `out.h5` is generated.


# Acknowledgment
We thank [Suman Ghosh](https://www.linkedin.com/in/suman-ghosh-a8762576/) from [tub-rip](https://github.com/tub-rip), and their tool [events_h52bag](https://github.com/tub-rip/events_h52bag) for converting event data from HDF5 to ROSbags.