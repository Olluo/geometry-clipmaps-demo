#ifndef TERRAIN_TESTING
#define TERRAIN_TESTING
#endif

#include <gtest/gtest.h>

#include "Manager.h"

namespace geoclipmap
{
  TEST(ManagerTest, instance)
  {
    Manager *manager = Manager::getInstance();
    Manager *manager2 = Manager::getInstance();
    EXPECT_EQ(manager, manager2);
  }

  TEST(ManagerTest, ctor)
  {
    Manager *manager = Manager::getInstance();

    EXPECT_EQ(manager->K(), 8);
    EXPECT_EQ(manager->D(), static_cast<size_t>(1) << 8);
    EXPECT_EQ(manager->N(), (static_cast<size_t>(1) << 8) - 1);
    EXPECT_EQ(manager->M(), (static_cast<size_t>(1) << 8) / 4);
    EXPECT_EQ(manager->D2(), (static_cast<size_t>(1) << 8) / 2);
    EXPECT_EQ(manager->H(), -2 * ((static_cast<size_t>(1) << 8) / 4) + 1);
    EXPECT_EQ(manager->L(), 8);
    EXPECT_EQ(manager->R(), 4);
  }

  TEST(ManagerTest, k_values)
  {
    Manager *manager = Manager::getInstance();

    // Valid value
    manager->setK(4);

    EXPECT_EQ(manager->K(), 4);
    EXPECT_EQ(manager->D(), static_cast<size_t>(1) << 4);
    EXPECT_EQ(manager->N(), (static_cast<size_t>(1) << 4) - 1);
    EXPECT_EQ(manager->M(), (static_cast<size_t>(1) << 4) / 4);
    EXPECT_EQ(manager->D2(), (static_cast<size_t>(1) << 4) / 2);
    EXPECT_EQ(manager->H(), -2 * ((static_cast<size_t>(1) << 4) / 4) + 1);

    // Below minimum
    manager->setK(2);

    EXPECT_EQ(manager->K(), 4);
    EXPECT_EQ(manager->D(), static_cast<size_t>(1) << 4);
    EXPECT_EQ(manager->N(), (static_cast<size_t>(1) << 4) - 1);
    EXPECT_EQ(manager->M(), (static_cast<size_t>(1) << 4) / 4);
    EXPECT_EQ(manager->D2(), (static_cast<size_t>(1) << 4) / 2);
    EXPECT_EQ(manager->H(), -2 * ((static_cast<size_t>(1) << 4) / 4) + 1);

    // Above maximum
    manager->setK(12);

    EXPECT_EQ(manager->K(), 10);
    EXPECT_EQ(manager->D(), static_cast<size_t>(1) << 10);
    EXPECT_EQ(manager->N(), (static_cast<size_t>(1) << 10) - 1);
    EXPECT_EQ(manager->M(), (static_cast<size_t>(1) << 10) / 4);
    EXPECT_EQ(manager->D2(), (static_cast<size_t>(1) << 10) / 2);
    EXPECT_EQ(manager->H(), -2 * ((static_cast<size_t>(1) << 10) / 4) + 1);
  }

  TEST(ManagerTest, l_values)
  {
    Manager *manager = Manager::getInstance();

    // Valid value
    manager->setL(4);

    EXPECT_EQ(manager->L(), 4);

    // Below minimum
    manager->setL(2);

    EXPECT_EQ(manager->L(), 4);

    // Above maximum
    manager->setL(14);

    EXPECT_EQ(manager->L(), 12);
  }

  TEST(ManagerTest, r_values)
  {
    Manager *manager = Manager::getInstance();

    // Valid value
    manager->setR(4);

    EXPECT_EQ(manager->R(), 4);

    // Below minimum
    manager->setR(0);

    EXPECT_EQ(manager->R(), 1);

    // Above maximum
    manager->setR(10);

    EXPECT_EQ(manager->R(), 8);
  }
} // end namespace geoclipmap