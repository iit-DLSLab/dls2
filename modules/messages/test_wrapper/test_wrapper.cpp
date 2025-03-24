#include "dls_messages/dds/testWrapper.hpp"
#include "robotlib/robot_factory.hpp"

#include <gtest/gtest.h>

dls::TestWrapper create_default_test(const std::shared_ptr<robotlib::RobotBase> pRobot){
    dls::TestWrapper test(pRobot);
    test.key = "key";
    test.array = {1.0, 2.0, 3.0};
    test.vector = {1, 2, 3};
    test.map_data = {{"key", 1.0}};
    test.header.frame_id = "frame_id";
    test.header.sequence_id = 1;
    test.header.timestamp = 0.1;
    test.mini_test.status = true;
    test.mini_test.number = 1.0;
    test.mini_test_robot.status = true;
    for(auto &pair : test.mini_test_robot.number){
        pair.getData() = std::vector<double>{1.0, 2.0, 3.0};
    }
    for(auto &pair : test.joint_state){
        pair.getData() = std::vector<double>{1.0, 2.0, 3.0};
    }
    for(auto &limb : test.limb_datamap){
        limb.getData() = true;
    }
    for(auto &limb : test.limb_datamap_eigenvec3){
        limb.getData() = Eigen::Vector3d(1.0, 2.0, 3.0);
    }
    return test;
}

void init_wrapper(dls::TestWrapper& wrapper, const std::shared_ptr<robotlib::RobotBase> pRobot){
    TestWrapper default_test = create_default_test(pRobot);
    wrapper.key = default_test.key;
    wrapper.array = default_test.array;
    wrapper.vector = default_test.vector;
    wrapper.map_data = default_test.map_data;
    wrapper.header = default_test.header;
    wrapper.mini_test = default_test.mini_test;
    wrapper.mini_test_robot = default_test.mini_test_robot;
    wrapper.joint_state = default_test.joint_state;
    wrapper.limb_datamap = default_test.limb_datamap;
    wrapper.limb_datamap_eigenvec3 = default_test.limb_datamap_eigenvec3;
}

void init_msg(Test& msg, const std::shared_ptr<robotlib::RobotBase> pRobot){
    TestWrapper default_test = create_default_test(pRobot);
    msg.key() = default_test.key;
    msg.array() = default_test.array;
    msg.vector() = default_test.vector;
    msg.map_data() = default_test.map_data;
    msg.header().frame_id() = default_test.header.frame_id;
    msg.header().sequence_id() = default_test.header.sequence_id;
    msg.header().timestamp() = default_test.header.timestamp;
    msg.mini_test().status() = default_test.mini_test.status;
    msg.mini_test().number() = default_test.mini_test.number;
    msg.mini_test_robot().status() = default_test.mini_test_robot.status;
    msg.mini_test_robot().number() = default_test.mini_test_robot.number.tovec_();
    msg.joint_state() = default_test.joint_state.tovec_();
    msg.limb_datamap() = default_test.limb_datamap.tovec_();
    for(auto &pair : default_test.limb_datamap_eigenvec3){
        msg.limb_datamap_eigenvec3().push_back(default_test.limb_datamap_eigenvec3[pair][0]);
        msg.limb_datamap_eigenvec3().push_back(default_test.limb_datamap_eigenvec3[pair][1]);
        msg.limb_datamap_eigenvec3().push_back(default_test.limb_datamap_eigenvec3[pair][2]);
    }
}

void test_wrapper(dls::TestWrapper& wrapper, const std::shared_ptr<robotlib::RobotBase> pRobot){
    TestWrapper default_test = create_default_test(pRobot);
    EXPECT_EQ(wrapper.key,default_test.key);
    for (long unsigned int i = 0; i < wrapper.array.size(); i++){
        EXPECT_EQ(wrapper.array[i],default_test.array[i]);
    }
    for (long unsigned int i = 0; i < wrapper.vector.size(); i++){
        EXPECT_EQ(wrapper.vector[i],default_test.vector[i]);
    }
    for(auto &pair : wrapper.map_data){
        EXPECT_EQ(pair.second,default_test.map_data.at(pair.first));
    }
    EXPECT_EQ(wrapper.header.frame_id,default_test.header.frame_id);
    EXPECT_EQ(wrapper.header.sequence_id,default_test.header.sequence_id);
    EXPECT_EQ(wrapper.header.timestamp,default_test.header.timestamp);
    EXPECT_EQ(wrapper.mini_test.status,default_test.mini_test.status);
    EXPECT_EQ(wrapper.mini_test.number,default_test.mini_test.number);
    EXPECT_EQ(wrapper.mini_test_robot.status,default_test.mini_test_robot.status);
    for(auto &pair : wrapper.mini_test_robot.number){
        EXPECT_EQ(pair.getData().tovec_()[0],default_test.mini_test_robot.number[pair].tovec_()[0]);
        EXPECT_EQ(pair.getData().tovec_()[1],default_test.mini_test_robot.number[pair].tovec_()[1]);
        EXPECT_EQ(pair.getData().tovec_()[2],default_test.mini_test_robot.number[pair].tovec_()[2]);
    }
    for(auto &pair : wrapper.joint_state){
        EXPECT_EQ(pair.getData().tovec_()[0],default_test.joint_state[pair].tovec_()[0]);
        EXPECT_EQ(pair.getData().tovec_()[1],default_test.joint_state[pair].tovec_()[1]);
        EXPECT_EQ(pair.getData().tovec_()[2],default_test.joint_state[pair].tovec_()[2]);
    }
    for(auto &limb : wrapper.limb_datamap){
        EXPECT_EQ(limb.getData(),default_test.limb_datamap[limb]);
    }
    for(auto &limb : wrapper.limb_datamap_eigenvec3){
        EXPECT_EQ(limb.getData()[0],default_test.limb_datamap_eigenvec3[limb][0]);
        EXPECT_EQ(limb.getData()[1],default_test.limb_datamap_eigenvec3[limb][1]);
        EXPECT_EQ(limb.getData()[2],default_test.limb_datamap_eigenvec3[limb][2]);
    }
}

void test_msg(const dls::TestWrapper::type& msg, const dls::TestWrapper& wrapper){
    EXPECT_EQ(msg.key(),wrapper.key);
    for (long unsigned int i = 0; i < msg.array().size(); i++){
        EXPECT_EQ(msg.array()[i],wrapper.array[i]);
    }
    for (long unsigned int i = 0; i < msg.vector().size(); i++){
        EXPECT_EQ(msg.vector()[i],wrapper.vector[i]);
    }
    for(auto &pair : msg.map_data()){
        EXPECT_EQ(pair.second,wrapper.map_data.at(pair.first));
    }
    EXPECT_EQ(msg.header().frame_id(),wrapper.header.frame_id);
    EXPECT_EQ(msg.header().sequence_id(),wrapper.header.sequence_id);
    EXPECT_EQ(msg.header().timestamp(),wrapper.header.timestamp);
    EXPECT_EQ(msg.mini_test().status(),wrapper.mini_test.status);
    EXPECT_EQ(msg.mini_test().number(),wrapper.mini_test.number);
    EXPECT_EQ(msg.mini_test_robot().status(),wrapper.mini_test_robot.status);
    std::vector<double> number_vec = wrapper.mini_test_robot.number.tovec_();
    for(long unsigned int i = 0; i < msg.mini_test_robot().number().size(); i++){
        EXPECT_EQ(msg.mini_test_robot().number()[i],number_vec[i]);
    }
    std::vector<double> joint_state_vec = wrapper.joint_state.tovec_();
    for(long unsigned int i = 0; i < msg.joint_state().size(); i++){
        EXPECT_EQ(msg.joint_state()[i],joint_state_vec[i]);
    }
    std::vector<bool> limb_datamap_vec = wrapper.limb_datamap.tovec_();
    for(long unsigned int i = 0; i < msg.limb_datamap().size(); i++){
        EXPECT_EQ(msg.limb_datamap()[i],limb_datamap_vec[i]);
    }
    std::vector<Eigen::Vector3d> limb_datamap_eigenvec3_vec = wrapper.limb_datamap_eigenvec3.tovec_();
    int i = 0;
    for(auto &limb: wrapper.limb_datamap_eigenvec3){
        EXPECT_EQ(msg.limb_datamap_eigenvec3()[i*3],wrapper.limb_datamap_eigenvec3[limb][0]);
        EXPECT_EQ(msg.limb_datamap_eigenvec3()[i*3+1],wrapper.limb_datamap_eigenvec3[limb][1]);
        EXPECT_EQ(msg.limb_datamap_eigenvec3()[i*3+2],wrapper.limb_datamap_eigenvec3[limb][2]);
        i++;
    }
}

TEST(TestWrapper, wrapper_init)
{
    std::shared_ptr<robotlib::RobotBase> pRobot;
    try
    {
        pRobot = robotlib::RobotFactory::openRobot(ROBOT);
    }
    catch (const std::exception &e)
    {
        std::cerr << "child_process: Could not open the robot " << ROBOT << std::endl;
        std::cerr << e.what() << std::endl;
    }

    dls::TestWrapper wrapper(pRobot);
    init_wrapper(wrapper, pRobot);
    test_wrapper(wrapper, pRobot);

    dls::TestWrapper wrapper2(wrapper);
    test_wrapper(wrapper2, pRobot);
}

TEST(TestWrapper, from_wrapper_to_msg)
{
    std::shared_ptr<robotlib::RobotBase> pRobot;
    try
    {
        pRobot = robotlib::RobotFactory::openRobot(ROBOT);
    }
    catch (const std::exception &e)
    {
        std::cerr << "child_process: Could not open the robot " << ROBOT << std::endl;
        std::cerr << e.what() << std::endl;
    }

    dls::TestWrapper wrapper(pRobot);
    init_wrapper(wrapper, pRobot);
    
    dls::TestWrapper::type msg;
    msg = wrapper;
    
    test_msg(msg,wrapper);
    
    dls::TestWrapper wrapper2(pRobot);
    init_wrapper(wrapper2, pRobot);
    dls::TestWrapper::type msg2;
    msg2 = (dls::TestWrapper::type) wrapper2;

    test_msg(msg2,wrapper2);
}

TEST(TestWrapper, from_msg_to_wrapper)
{
    std::shared_ptr<robotlib::RobotBase> pRobot;
    try
    {
        pRobot = robotlib::RobotFactory::openRobot(ROBOT);
    }
    catch (const std::exception &e)
    {
        std::cerr << "child_process: Could not open the robot " << ROBOT << std::endl;
        std::cerr << e.what() << std::endl;
    }

    dls::TestWrapper::type msg;
    init_msg(msg,pRobot);

    dls::TestWrapper wrapper(pRobot);
    wrapper = msg;
    
    test_wrapper(wrapper,pRobot);
}

TEST(TestWrapper, from_wrapper_to_wrapper)
{
    std::shared_ptr<robotlib::RobotBase> pRobot;
    try
    {
        pRobot = robotlib::RobotFactory::openRobot(ROBOT);
    }
    catch (const std::exception &e)
    {
        std::cerr << "child_process: Could not open the robot " << ROBOT << std::endl;
        std::cerr << e.what() << std::endl;
    }

    dls::TestWrapper wrapper(pRobot);
    init_wrapper(wrapper,pRobot);

    dls::TestWrapper wrapper2(pRobot);
    wrapper2 = wrapper;

    test_wrapper(wrapper2,pRobot);
}

TEST(TestWrapper, setDataFromWrapperBase)
{
    std::cout << "\nTEST FOR setDataFromWrapperBase IS NOT IMPLEMENTED, SKIPPING...\n" << std::endl;
}