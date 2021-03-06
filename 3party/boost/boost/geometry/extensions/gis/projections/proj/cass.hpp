#ifndef BOOST_GEOMETRY_PROJECTIONS_CASS_HPP
#define BOOST_GEOMETRY_PROJECTIONS_CASS_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_mlfn.hpp>

#include <boost/geometry/extensions/gis/projections/epsg_traits.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace cass
    {

            static const double EPS10 = 1e-10;
            static const double C1 = .16666666666666666666;
            static const double C2 = .00833333333333333333;
            static const double C3 = .04166666666666666666;
            static const double C4 = .33333333333333333333;
            static const double C5 = .06666666666666666666;

            struct par_cass
            {
                double m0;
                double en[EN_SIZE];
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_cass_ellipsoid : public base_t_fi<base_cass_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_cass m_proj_parm;

                inline base_cass_ellipsoid(const Parameters& par)
                    : base_t_fi<base_cass_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                // FORWARD(e_forward)  ellipsoid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double n = sin(lp_lat);
                    double c = cos(lp_lat);
                    xy_y = pj_mlfn(lp_lat, n, c, this->m_proj_parm.en);
                    n = 1./sqrt(1. - this->m_par.es * n * n);
                    double tn = tan(lp_lat); double t = tn * tn;
                    double a1 = lp_lon * c;
                    c *= this->m_par.es * c / (1 - this->m_par.es);
                    double a2 = a1 * a1;
                    xy_x = n * a1 * (1. - a2 * t *
                        (C1 - (8. - t + 8. * c) * a2 * C2));
                    xy_y -= this->m_proj_parm.m0 - n * tn * a2 *
                        (.5 + (5. - t + 6. * c) * a2 * C3);
                }

                // INVERSE(e_inverse)  ellipsoid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double ph1;

                    ph1 = pj_inv_mlfn(this->m_proj_parm.m0 + xy_y, this->m_par.es, this->m_proj_parm.en);
                    double tn = tan(ph1); double t = tn * tn;
                    double n = sin(ph1);
                    double r = 1. / (1. - this->m_par.es * n * n);
                    n = sqrt(r);
                    r *= (1. - this->m_par.es) * n;
                    double dd = xy_x / n;
                    double d2 = dd * dd;
                    lp_lat = ph1 - (n * tn / r) * d2 *
                        (.5 - (1. + 3. * t) * d2 * C3);
                    lp_lon = dd * (1. + t * d2 *
                        (-C4 + (1. + 3. * t) * d2 * C5)) / cos(ph1);
                }

                static inline std::string get_name()
                {
                    return "cass_ellipsoid";
                }

            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_cass_spheroid : public base_t_fi<base_cass_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_cass m_proj_parm;

                inline base_cass_spheroid(const Parameters& par)
                    : base_t_fi<base_cass_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                // FORWARD(s_forward)  spheroid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    xy_x = asin(cos(lp_lat) * sin(lp_lon));
                    xy_y = atan2(tan(lp_lat) , cos(lp_lon)) - this->m_par.phi0;
                }

                // INVERSE(s_inverse)  spheroid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double dd = xy_y + this->m_par.phi0;
                    lp_lat = asin(sin(dd) * cos(xy_x));
                    lp_lon = atan2(tan(xy_x), cos(dd));
                }

                static inline std::string get_name()
                {
                    return "cass_spheroid";
                }

            };

            // Cassini
            template <typename Parameters>
            void setup_cass(Parameters& par, par_cass& proj_parm)
            {
                if (par.es) {
                    if (!pj_enfn(par.es, proj_parm.en)) throw proj_exception(0);
                    proj_parm.m0 = pj_mlfn(par.phi0, sin(par.phi0), cos(par.phi0), proj_parm.en);
                } else {
                }
            }

        }} // namespace detail::cass
    #endif // doxygen

    /*!
        \brief Cassini projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
        \par Example
        \image html ex_cass.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct cass_ellipsoid : public detail::cass::base_cass_ellipsoid<Geographic, Cartesian, Parameters>
    {
        inline cass_ellipsoid(const Parameters& par) : detail::cass::base_cass_ellipsoid<Geographic, Cartesian, Parameters>(par)
        {
            detail::cass::setup_cass(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Cassini projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
        \par Example
        \image html ex_cass.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct cass_spheroid : public detail::cass::base_cass_spheroid<Geographic, Cartesian, Parameters>
    {
        inline cass_spheroid(const Parameters& par) : detail::cass::base_cass_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::cass::setup_cass(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class cass_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    if (par.es)
                        return new base_v_fi<cass_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                    else
                        return new base_v_fi<cass_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void cass_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("cass", new cass_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    // Create EPSG specializations
    // (Proof of Concept, only for some)

    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<2066, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=11.25217861111111 +lon_0=-60.68600888888889 +x_0=37718.66159325 +y_0=36209.91512952 +a=6378293.645208759 +b=6356617.987679838 +to_meter=0.201166195164";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<2099, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=25.38236111111111 +lon_0=50.76138888888889 +x_0=100000 +y_0=100000 +ellps=helmert +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<2314, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=10.44166666666667 +lon_0=-61.33333333333334 +x_0=86501.46392052001 +y_0=65379.0134283 +a=6378293.645208759 +b=6356617.987679838 +to_meter=0.3047972654";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3068, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=52.41864827777778 +lon_0=13.62720366666667 +x_0=40000 +y_0=10000 +ellps=bessel +datum=potsdam +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3140, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=-18 +lon_0=178 +x_0=109435.392 +y_0=141622.272 +a=6378306.3696 +b=6356571.996 +towgs84=51,391,-36,0,0,0,0 +to_meter=0.201168";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3366, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=22.31213333333334 +lon_0=114.1785555555556 +x_0=40243.57775604237 +y_0=19069.93351512578 +a=6378293.645208759 +b=6356617.987679838 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3377, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=2.121679744444445 +lon_0=103.4279362361111 +x_0=-14810.562 +y_0=8758.32 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3378, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=2.682347636111111 +lon_0=101.9749050416667 +x_0=3673.785 +y_0=-4240.573 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3379, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=3.769388088888889 +lon_0=102.3682989833333 +x_0=-7368.228 +y_0=6485.858 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3380, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=3.68464905 +lon_0=101.3891079138889 +x_0=-34836.161 +y_0=56464.049 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3381, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=4.9762852 +lon_0=103.070275625 +x_0=19594.245 +y_0=3371.895 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3382, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=5.421517541666667 +lon_0=100.3443769638889 +x_0=-23.414 +y_0=62.283 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3383, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=5.964672713888889 +lon_0=100.6363711111111 +x_0=0 +y_0=0 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3384, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=4.859063022222222 +lon_0=100.8154105861111 +x_0=-1.769 +y_0=133454.779 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3385, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=5.972543658333334 +lon_0=102.2952416694444 +x_0=13227.851 +y_0=8739.894 +ellps=GRS80 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<3407, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=22.31213333333334 +lon_0=114.1785555555556 +x_0=40243.57775604237 +y_0=19069.93351512578 +a=6378293.645208759 +b=6356617.987679838 +to_meter=0.3047972654";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<24500, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=1.287646666666667 +lon_0=103.8530022222222 +x_0=30000 +y_0=30000 +a=6377304.063 +b=6356103.038993155 +towgs84=-11,851,5,0,0,0,0 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<28191, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=31.73409694444445 +lon_0=35.21208055555556 +x_0=170251.555 +y_0=126867.909 +a=6378300.789 +b=6356566.435 +towgs84=-275.722,94.7824,340.894,-8.001,-4.42,-11.821,1 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<28193, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=31.73409694444445 +lon_0=35.21208055555556 +x_0=170251.555 +y_0=1126867.909 +a=6378300.789 +b=6356566.435 +towgs84=-275.722,94.7824,340.894,-8.001,-4.42,-11.821,1 +units=m";
        }
    };


    template<typename LatLongRadian, typename Cartesian, typename Parameters>
    struct epsg_traits<30200, LatLongRadian, Cartesian, Parameters>
    {
        typedef cass_ellipsoid<LatLongRadian, Cartesian, Parameters> type;
        static inline std::string par()
        {
            return "+proj=cass +lat_0=10.44166666666667 +lon_0=-61.33333333333334 +x_0=86501.46392051999 +y_0=65379.0134283 +a=6378293.645208759 +b=6356617.987679838 +to_meter=0.201166195164";
        }
    };


    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_CASS_HPP

