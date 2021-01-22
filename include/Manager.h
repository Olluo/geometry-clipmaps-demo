/**
 * @file Manager.h
 * @author Ollie Nicholls
 * @brief This class implements a singleton pattern to manage the constants used
 *  by the geoclipmap
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
    // This class shouldn't be copyable
    Manager(Manager & /*other*/) = delete;
    // This class shouldn't be copy assignable
    void operator=(const Manager & /*other*/) = delete;

    /**
     * @brief Get an instance of the manager if it exists or create one if it 
     * doesn't
     * 
     * @return Manager* a reference to the manager
     */
    static Manager *getInstance();
    /**
     * @brief Set the K value (level of detail) and all other variables that 
     * rely on K
     * 
     * @param _k The new K value
     */
    void setK(unsigned char _k);
    /**
     * @brief Set the L value (number of levels)
     * 
     * @param _l The new L value
     */
    void setL(unsigned char _l);
    /**
     * @brief Set the R value (The number of levels to show)
     * 
     * @param _r The new R value
     */
    void setR(unsigned char _r);

    /**
     * @brief Get the K value (level of detail)
     */
    unsigned char K();
    /**
     * @brief Get the D value (2 ^ K)
     */
    size_t D();
    /**
     * @brief Get the N value (D - 1)
     */
    size_t N();
    /**
     * @brief Get the L value (number of levels)
     */
    unsigned char L();
    /**
     * @brief Get the M value (D / 4)
     */
    size_t M();
    /**
     * @brief Get the D2 value (D / 2)
     */
    size_t D2();
    /**
     * @brief Get the H value (Clipmap level center)
     */
    long H();
    /**
     * @brief Get the R value (The number of levels to show)
     */
    unsigned char R();

  private:
    Manager(){};
    static Manager *m_instance;

    // K - The level of detail (>3)
    unsigned char m_K = 8;
    // Minimum value K can take (>3)
    unsigned char m_KMin = 4;
    // The maximum value K can take (<11 to avoid lag)
    unsigned char m_KMax = 10;
    // D - Equivalent of 2^K
    size_t m_D = static_cast<size_t>(1) << m_K;
    // N - Grid size (must be odd hence -1)
    size_t m_N = m_D - 1;
    // M -
    size_t m_M = m_D / 4;
    // D/2
    size_t m_D2 = m_D / 2;
    // H - How much to move the clipmap by to find centre point
    long m_H = -2 * static_cast<int>(m_M) + 1;
    // L - The number of levels of detail (>3)
    unsigned char m_L = 8;
    // Minimum value of L (>3 or program crash)
    unsigned char m_LMin = 4;
    // The maximum value L (<13 to avoid lag)
    unsigned char m_LMax = 12;
    // R - the number of levels to show between finest and coarsest
    unsigned char m_R = 4;
    // The minimum value of R
    // can't have any less that 1 level of detail between coarsest and finest
    unsigned char m_RMin = 1;
    // The maximum value of R, any higher and the program can crash
    unsigned char m_RMax = 8;
  };

} // end namespace geoclipmap
#endif // !MANAGER_H_