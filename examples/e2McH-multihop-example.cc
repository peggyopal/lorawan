/*
 * This example is based off of the paper:
 *   https://ieeexplore.ieee.org/document/8436722
 *
 * Class C LoRa Nodes are used with an assumed battery capacity of 2000 mAh
 * LoRa modules words on two different frequencies: 868 MHz and 433MHz
 * Sending frequency is 2 packets/second
 * Two scenarios are simulated:
 *   1) 2 LoRa Nodes, one Node has a distance of 12km from the gateway, the
 *      other node is 5km from first node and 8km from the gateway
 *   2) 7 LoRa Nodes with varying distances from gateway (within max 12km radius
 *      centered on gateway)
 */

using namespace ns3;
using namespace lorawan;

NS_LOG_COMPONENT_DEFINE ("e2McHMultihopExample");

int
main (int argc, char *argv[])
{
    return 0;
}
