# events_bag2h5
This tool coverts event messages from ROSbags to HDF5, and keeps the original C++ code to convert event data from HDF5 to ROSbags. 

More information about Installation and details about h52bag can be found in this [repo](https://github.com/tub-rip/events_h52bag).

## Installation

(1) Do not forget to add path to your installed `dvs_msgs` header in line 11 of `CMakeLists.txt`. 
If not already installed, install `dvs_msgs` from https://github.com/uzh-rpg/rpg_dvs_ros in your catkin workspace.

(2) Clone this repository: 

`git clone https://github.com/atkai/events_bag2h5`

(3) Compile this repository:
```
cd events_bag2h5
mkdir build && cd build
cmake .. & make -j6
```


## Run
 (1) How to use this tool
 
`./events_bag2h5 <path/to/input/bagfile> <path/to/output/h5file(without extension)> <event_topic_name> [optional, default="events"]<dataset_prefix>`

(2) An example

* Download input data from DAVIS240C dataset: https://rpg.ifi.uzh.ch/datasets/davis/boxes_6dof.bag
* Execute: `./events_bag2h5 boxes_6dof.bag out /dvs/events`
* The output ROSBag `out.h5` is generated.


## Acknowledgment
This tool is derived from [events_h52bag](https://github.com/tub-rip/events_h52bag) ([Suman Ghosh](https://www.linkedin.com/in/suman-ghosh-a8762576/) from [tub-rip](https://github.com/tub-rip)) for converting event data from HDF5 to ROSbags.
