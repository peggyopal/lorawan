/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This file includes testing for the following components:
 * - ClassCEndDeviceStatus
 *
 * Author:Peggy Anderson <peggy.anderson@usask.ca>
*/

// Include headers of classes to test
#include "ns3/log.h"
#include "ns3/class-c-end-device-lorawan-mac.h"

// An essential include is test.h
#include "ns3/test.h"

using namespace ns3;
using namespace lorawan;

NS_LOG_COMPONENT_DEFINE ("ClassCEndDeviceTestSuite");

/////////////////////////////
// EndDeviceStatus testing //
/////////////////////////////

class ClassCEndDeviceTestSuite : public TestCase
{
public:
  ClassCEndDeviceTestSuite ();
  virtual ~ClassCEndDeviceTestSuite ();

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
ClassCEndDeviceTestSuite::ClassCEndDeviceTestSuite ()
  : TestCase ("Verify that the ClassCEndDeviceLorawan operates as expected")
{
}

// Reminder that the test case should clean up after itself
ClassCEndDeviceTestSuite::~ClassCEndDeviceTestSuite ()
{
}

// This method is the pure virtual method from class TestCase that every
// TestCase must implement
void
ClassCEndDeviceTestSuite::DoRun (void)
{
  NS_LOG_DEBUG ("ClassCEndDeviceTestSuite");
}
