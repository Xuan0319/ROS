#include <ros/ros.h>
#include <GPS_control_functions.hpp>
#include <std_msgs/String.h>
#include <iostream>


std::vector<gnc_api_waypoint> waypointlist;
gnc_api_waypoint nextwp;

int move(float lat,float lon,float alt){

    nextwp.lat = lat;
    nextwp.lon = lon;
    nextwp.alt = alt;
    waypointlist.push_back(nextwp);

	return 0;
 }


int start_mission(){

    ros::Rate rate(2.0);
    wait4connect();   

	//set_mode("GUIDED");	

	//create local reference frame 
	initialize_local_frame();	
	// arm();
	// takeoff(1);

	auto globel=get_position();

	move(globel.lat,globel.lon,2);
	move(globel.lat + 0.00003 ,globel.lon + 0.0 , 2);
	move(globel.lat + 0.00003 ,globel.lon + 0.00003,2);
	move(globel.lat + 0.0 ,globel.lon + 0.00003 ,2);
	move(globel.lat,globel.lon,2);

    
    int counter = 0;
    //wait for position information
    while(counter < waypointlist.size()) 
    {
		set_waypoint(waypointlist[counter].lat,waypointlist[counter].lon,waypointlist[counter].alt);
		ROS_INFO("GPS position sent: [%f %f %f]", waypointlist[counter].lat, waypointlist[counter].lon, waypointlist[counter].alt);
		// counter++;
		// sleep(4);
		while (true)	
		{		
		if (check_position(waypointlist[counter].lat, waypointlist[counter].lon)== 1)
		{
			ROS_INFO("ok");
			counter++;
			break;
		}
		}
		sleep(5);	

	}
return 0;
}

void cmd_receiver(const std_msgs::String msg){
	std::cout << msg << std::endl;
	mqtt_command.append(msg.data);
	if(mqtt_command=="takeoff"){
		ROS_INFO("get_cmd_takeoff");
		set_mode("GUIDED");
		arm();
		takeoff(1);
		mqtt_command="";
	}
	if(mqtt_command=="go"){
		ROS_INFO("get_cmd_go");
		start_mission();
		mqtt_command="";
	}
	if(mqtt_command=="land"){
		ROS_INFO("get_cmd_land");
		land();
		mqtt_command="";
	}
}



int main(int argc, char *argv[])
{
	std::string nodeName = "gnc_node";
	ros::init(argc, argv, nodeName);
	ros::NodeHandle gnc_node;
	//initialize control publisher/subscribers
	init_publisher_subscriber(gnc_node);

	const ros::Subscriber mqtt_sub = gnc_node.subscribe<std_msgs::String>("cmd_receiver",1000,cmd_receiver);

	//start_mission();
	ros::spin();
    //return 0;
}
