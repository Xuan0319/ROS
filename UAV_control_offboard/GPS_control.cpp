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
    nextwp.x = -35.3632622;
    nextwp.y = 149.1652375;
    nextwp.z = 5;
    waypointlist.push_back(nextwp);

    nextwp.x = -35.3632722;
    nextwp.y = 149.1652375;
    nextwp.z = 5;
    waypointlist.push_back(nextwp);

    // nextwp.x = -35.3613530;
    // nextwp.y = 149.1579735;
    // nextwp.z = 5;
    // waypointlist.push_back(nextwp);

    // nextwp.x = -35.3613535;
    // nextwp.y = 149.1579730;
    // nextwp.z = 5;
    // waypointlist.push_back(nextwp);
    
    int i = 0;
    //wait for position information
    while(ros::ok()) 
    {

	if(i < waypointlist.size())
	{
		set_waypoint(waypointlist[i].x,waypointlist[i].y,waypointlist[i].z);
		ROS_INFO("GPS position sent: [%f %f %f]", waypointlist[i].x, waypointlist[i].y, waypointlist[i].z);
		i++;
		sleep(4);
		/*while (waypoint_reached(waypointlist[i].lat, waypointlist[i].lon)!= 1)	
		{		
		if (waypoint_reached(waypointlist[i].lat, waypointlist[i].lon)== 1)
		{
			i++;
			break;
		}
		else
		{
			continue;		
		}}*/
		//sleep(5);	
	}
	else
	{
		land();
	} 
       
    }
    return 0;
}