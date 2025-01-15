#include <string>
#include <hdf5/serial/hdf5.h>
#include <hdf5/serial/H5Cpp.h>
#include <vector>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <dvs_msgs/Event.h>
#include <dvs_msgs/EventArray.h>

#ifdef OLD_HEADER_FILENAME
#else
#include <iostream>
#endif

using std::cout;
using std::endl;
using namespace H5;


template <typename T>
void appendH5Datasets(H5File &file, const std::string &datasetName, const std::vector<T> &data){

    DataSet dataset;
    if(!H5Lexists(file.getId(), datasetName.c_str(), H5P_DEFAULT)){
        hsize_t dims[1] = {data.size()};
        hsize_t maxdims[1] = {H5S_UNLIMITED};
        DataSpace dataspace(1, dims, maxdims);

        DSetCreatPropList plist;
        hsize_t chunkdims[1] = {100000};
        plist.setChunk(1, chunkdims);

        if(std::is_same<T, uint64_t>::value){
            dataset = file.createDataSet(datasetName, PredType::NATIVE_UINT64, dataspace, plist);
            dataset.write(data.data(), PredType::NATIVE_UINT64);
        } else if(std::is_same<T, uint16_t>::value){
            dataset = file.createDataSet(datasetName, PredType::NATIVE_UINT16, dataspace, plist);
            dataset.write(data.data(), PredType::NATIVE_UINT16);
        } else if(std::is_same<T, uint8_t>::value){
            dataset = file.createDataSet(datasetName, PredType::NATIVE_UINT8, dataspace, plist);
            dataset.write(data.data(), PredType::NATIVE_UINT8);
        }

        dataset.close();

    }else{
        dataset = file.openDataSet(datasetName.c_str());

        // Get current size of dataset
        DataSpace dataspace = dataset.getSpace();
        hsize_t current_size[1] = {0};
        dataspace.getSimpleExtentDims(current_size);

        // Calculate new size of dataset
        hsize_t new_size = data.size() + current_size[0];

        // Extend the dataset
        dataset.extend(&new_size);

        hsize_t offset[1] = {current_size[0]};
        hsize_t dims[1] = {data.size()};

        dataspace = dataset.getSpace();
        dataspace.selectHyperslab(H5S_SELECT_SET, dims, offset);

        DataSpace memspace(1, dims);

        // Define the memory dataspace
        if(std::is_same<T, uint64_t>::value){
            dataset.write(data.data(), PredType::NATIVE_UINT64, memspace, dataspace);
        } else if(std::is_same<T, uint16_t>::value){
            dataset.write(data.data(), PredType::NATIVE_UINT16, memspace, dataspace);
        } else if(std::is_same<T, uint8_t>::value){
            dataset.write(data.data(), PredType::NATIVE_UINT8, memspace, dataspace);
        }

        dataset.close();

    }

}


int main(int argc, char *argv[]){

    std::string dataset_prefix = "events";
    if (argc<4){
        cout<<"Too few arguments.. Exiting!\n";
        return 0;
    }
    std::string bagfile = argv[1];
    std::string fname = argv[2];
    std::string topic = argv[3];

    if (argc == 5){
        dataset_prefix = argv[4];
    }


    /*
        std::string dataset_prefix = "prophesee/right";
        * Try block to detect exceptions raised by any of the calls inside it
        */
    try
    {
        H5File file(fname + ".h5", H5F_ACC_TRUNC);
        file.createGroup(dataset_prefix.c_str());

        rosbag::Bag bag;
        bag.open(bagfile, rosbag::bagmode::Read);

        rosbag::View view(bag, rosbag::TopicQuery(topic));

        cout << "Creating HDF5 file..." << endl;
        cout << "Total event messages: " << view.size() << endl;
        for(const rosbag::MessageInstance& m : view){
            dvs_msgs::EventArray::ConstPtr s = m.instantiate<dvs_msgs::EventArray>();
            if (s != NULL){
                std::vector<uint64_t> t;
                std::vector<uint16_t> x;
                std::vector<uint16_t> y;
                std::vector<uint8_t> p;
                std::vector<uint64_t> offset_data;
                for (size_t i=0; i<s->events.size(); ++i){
                    t.push_back(s->events[i].ts.toSec()*1e6);
                    x.push_back(s->events[i].x);
                    y.push_back(s->events[i].y);
                    p.push_back(s->events[i].polarity);
                    offset_data.push_back(0);
                }

                appendH5Datasets(file, dataset_prefix + "/t", t);
                appendH5Datasets(file, dataset_prefix + "/x", x);
                appendH5Datasets(file, dataset_prefix + "/y", y);
                appendH5Datasets(file, dataset_prefix + "/p", p);
                appendH5Datasets(file, "t_offset", offset_data);
            }
        }

        cout << "HDF5 file created successfully!" << endl;

        bag.close();
        file.close();

    }  // end of try block
    // catch failure caused by the H5File operations
    catch( FileIException error )
    {
        error.printErrorStack();
        return -1;
    }
    // catch failure caused by the DataSet operations
    catch( DataSetIException error )
    {
        error.printErrorStack();
        return -1;
    }
    // catch failure caused by the DataSpace operations
    catch( DataSpaceIException error )
    {
        error.printErrorStack();
        return -1;
    }
    // catch failure caused by the DataSpace operations
    catch( DataTypeIException error )
    {
        error.printErrorStack();
        return -1;
    }
    catch(rosbag::BagException error){
        cout<<error.what()<<endl;
        return -1;
    }

    return 0;  // successfully terminated
}
