#include "Manager.h"

#include <algorithm>

namespace geoclipmap
{
  Manager *Manager::m_instance = 0;

  Manager *Manager::getInstance()
  {
    if (!m_instance)
    {
      m_instance = new Manager();
      return m_instance;
    }
    else
    {
      return m_instance;
    }
  }

  void Manager::setK(unsigned char _k)
  {
    m_K = std::clamp(_k, m_KMin, m_KMax);
    m_D = static_cast<size_t>(1) << m_K;
    m_N = m_D - 1;
    m_M = m_D / 4;
    m_D2 = m_D / 2;
    m_H = -2 * static_cast<int>(m_M) + 1;
  }

  void Manager::setL(unsigned char _l)
  {
    m_L = std::clamp(_l, m_LMin, m_LMax);
  }

  void Manager::setR(unsigned char _r)
  {
    m_R = std::clamp(_r, m_RMin, m_RMax);
  }

  unsigned char Manager::K()
  {
    return m_K;
  }

  size_t Manager::D()
  {
    return m_D;
  }

  size_t Manager::N()
  {
    return m_N;
  }

  unsigned char Manager::L()
  {
    return m_L;
  }

  size_t Manager::M()
  {
    return m_M;
  }

  size_t Manager::D2()
  {
    return m_D2;
  }

  long Manager::H()
  {
    return m_H;
  }

  unsigned char Manager::R()
  {
    return m_R;
  }

} // end namespace geoclipmap