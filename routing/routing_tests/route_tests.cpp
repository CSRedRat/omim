#include "testing/testing.hpp"

#include "routing/route.hpp"
#include "routing/turns.hpp"

#include "platform/location.hpp"

#include "geometry/point2d.hpp"

#include "std/string.hpp"
#include "std/vector.hpp"

using namespace routing;

namespace
{
static vector<m2::PointD> const kTestGeometry({{0, 0}, {0,1}, {1,1}, {1,2}, {1,3}});
static vector<turns::TurnItem> const kTestTurns({turns::TurnItem(1, turns::TurnDirection::TurnLeft),
                               turns::TurnItem(2, turns::TurnDirection::TurnRight),
                               turns::TurnItem(4, turns::TurnDirection::ReachedYourDestination)});

location::GpsInfo GetGps(double x, double y)
{
  location::GpsInfo info;
  info.m_latitude = MercatorBounds::YToLat(y);
  info.m_longitude = MercatorBounds::XToLon(x);
  info.m_horizontalAccuracy = 2;
  info.m_speed = -1;
  return info;
}
}  // namespace

UNIT_TEST(AddAdsentCountryToRouteTest)
{
  Route route("TestRouter");
  route.AddAbsentCountry("A");
  route.AddAbsentCountry("A");
  route.AddAbsentCountry("B");
  route.AddAbsentCountry("C");
  route.AddAbsentCountry("B");
  set<string> const & absent = route.GetAbsentCountries();
  TEST(absent.find("A") != absent.end(), ());
  TEST(absent.find("B") != absent.end(), ());
  TEST(absent.find("C") != absent.end(), ());
}

UNIT_TEST(DistanceToCurrentTurnTest)
{
  Route route("TestRouter");
  route.SetGeometry(kTestGeometry.begin(), kTestGeometry.end());
  vector<turns::TurnItem> turns(kTestTurns);
  route.SetTurnInstructions(turns);

  double distance;
  turns::TurnItem turn;

  route.GetCurrentTurn(distance, turn);
  TEST(my::AlmostEqualAbs(distance,
                          MercatorBounds::DistanceOnEarth(kTestGeometry[0], kTestGeometry[1]), 0.1),
                          ());
  TEST_EQUAL(turn, kTestTurns[0], ());

  route.MoveIterator(GetGps(0, 0.5));
  route.GetCurrentTurn(distance, turn);
  TEST(my::AlmostEqualAbs(distance,
                          MercatorBounds::DistanceOnEarth({0, 0.5}, kTestGeometry[1]), 0.1), ());
  TEST_EQUAL(turn, kTestTurns[0], ());

  route.MoveIterator(GetGps(1, 1.5));
  route.GetCurrentTurn(distance, turn);
  TEST(my::AlmostEqualAbs(distance,
                          MercatorBounds::DistanceOnEarth({1, 1.5}, kTestGeometry[4]), 0.1), ());
  TEST_EQUAL(turn, kTestTurns[2], ());

  route.MoveIterator(GetGps(1, 2.5));
  route.GetCurrentTurn(distance, turn);
  TEST(my::AlmostEqualAbs(distance,
                          MercatorBounds::DistanceOnEarth({1, 2.5}, kTestGeometry[4]), 0.1), ());
  TEST_EQUAL(turn, kTestTurns[2], ());
}

UNIT_TEST(NextTurnTest)
{
  Route route("TestRouter");
  route.SetGeometry(kTestGeometry.begin(), kTestGeometry.end());
  vector<turns::TurnItem> turns(kTestTurns);
  route.SetTurnInstructions(turns);

  double distance, nextDistance;
  turns::TurnItem turn;
  turns::TurnItem nextTurn;

  route.GetCurrentTurn(distance, turn);
  route.GetNextTurn(nextDistance, nextTurn);
  TEST_EQUAL(turn, kTestTurns[0], ());
  TEST_EQUAL(nextTurn, kTestTurns[1], ());

  route.MoveIterator(GetGps(0.5, 1));
  route.GetCurrentTurn(distance, turn);
  route.GetNextTurn(nextDistance, nextTurn);
  TEST_EQUAL(turn, kTestTurns[1], ());
  TEST_EQUAL(nextTurn, kTestTurns[2], ());

  route.MoveIterator(GetGps(1, 1.5));
  route.GetCurrentTurn(distance, turn);
  route.GetNextTurn(nextDistance, nextTurn);
  TEST_EQUAL(turn, kTestTurns[2], ());
  TEST_EQUAL(nextTurn, turns::TurnItem(), ());
}
