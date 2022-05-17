#include <ros/ros.h>
#include <GPS_control_functions.hpp>


int main(int argc, char **argv)
{
	ros::init(argc, argv, "gnc_node");
	ros::NodeHandle gnc_node("~");
	
	//initialize control publisher/subscribers
	init_publisher_subscriber(gnc_node);


    ros::Rate rate(2.0);
    wait4connect();   

	set_mode("GUIDED");	

	//create local reference frame 
	initialize_local_frame();

	arm();

	//request takeoff
	takeoff(1);
	
	std::vector<gnc_api_waypoint> waypointlist;
    gnc_api_waypoint nextwp;

	auto globel=get_position();
    nextwp.lat = globel.lat;
    nextwp.lon = globel.lon;
    nextwp.alt = 2;
    waypointlist.push_back(nextwp);

    nextwp.lat = globel.lat + 0.00003;
    nextwp.lon = globel.lon + 0.0;
    nextwp.alt = 2;
    waypointlist.push_back(nextwp);

    nextwp.lat = globel.lat + 0.00003;
    nextwp.lon = globel.lon + 0.00003;
    nextwp.alt = 2;
    waypointlist.push_back(nextwp);

	nextwp.lat = globel.lat + 0.00000;
    nextwp.lon = globel.lon + 0.00003;
    nextwp.alt = 2;
    waypointlist.push_back(nextwp);

	nextwp.lat = globel.lat + 0.00000;
    nextwp.lon = globel.lon + 0.00000;
    nextwp.alt = 2;
    waypointlist.push_back(nextwp);
    
    int counter = 0;
    //wait for position information
    while(ros::ok()) 
    {

	if(counter < waypointlist.size())
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
	else
	{
		land();
	} 
       
    }
    return 0;
}
