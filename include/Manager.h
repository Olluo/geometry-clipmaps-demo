/**
 * @file Manager.h
 * @author Ollie Nicholls
 * @brief 
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MANAGER_H_
#define MANAGER_H_

namespace geoclipmap
{
  class Manager
  {
  public:
    Manager(Manager & /*other*/) = delete;
    void operator=(const Manager & /*other*/) = delete;

    static Manager *getInstance();

    void setK(unsigned char _k);
    void setL(unsigned char _l);
    void setR(unsigned char _r);

    unsigned char K();
    size_t D();
    size_t N();
    unsigned char L();
    size_t M();
    size_t D2();
    long H();
    unsigned char R();

  private:
    Manager(){};
    static Manager *m_instance;

    // K - The level of detail (>3) - 4 for testing, 8 or 16 for final - tested values 4 to 9, any higher and program crashes
    unsigned char m_K = 9;
    unsigned char m_KMin = 4;
    unsigned char m_KMax = 10;
    // D - Equivalent of 2^K
    size_t m_D = static_cast<size_t>(1) << m_K;
    // N - Grid size (must be odd hence -1)
    size_t m_N = m_D - 1;
    // M -
    size_t m_M = m_D / 4;
    // D/2
    size_t m_D2 = m_D / 2;
    // H - How much to move the clipmap by to find center point
    long m_H = -2 * static_cast<int>(m_M) + 1;
    // L - The number of levels of detail (>3) - 4 for testing 11 or higher final
    unsigned char m_L = 3;
    unsigned char m_LMin = 4;
    unsigned char m_LMax = 12;
    // R - the number of levels to show between finest and coarsest
    unsigned char m_R = 4;
    unsigned char m_RMin = 1;
    unsigned char m_RMax = 8;
  };

} // end namespace geoclipmap
#endif // !MANAGER_H_