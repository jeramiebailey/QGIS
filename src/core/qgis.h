/***************************************************************************
                          qgis.h - QGIS namespace
                             -------------------
    begin                : Sat Jun 30 2002
    copyright            : (C) 2002 by Gary E.Sherman
    email                : sherman at mrcc.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGIS_H
#define QGIS_H

#include <QMetaEnum>
#include <cfloat>
#include <memory>
#include <cmath>

#include "qgstolerance.h"
#include "qgis_core.h"
#include "qgis_sip.h"

#ifdef SIP_RUN
% ModuleHeaderCode
#include <qgis.h>
% End

% ModuleCode
int QgisEvent = QEvent::User + 1;
% End
#endif


/**
 * \ingroup core
 * \brief The Qgis class provides global constants for use throughout the application.
 */
class CORE_EXPORT Qgis
{
    Q_GADGET
  public:

    /**
     * Version string.
     *
     * \since QGIS 3.12
     */
    static QString version();

    /**
     * Version number used for comparing versions using the "Check QGIS Version" function
     *
     * \since QGIS 3.12
     */
    static int versionInt();

    /**
     * Release name
     *
     * \since QGIS 3.12
     */
    static QString releaseName();

    //! The development version
    static const char *QGIS_DEV_VERSION;

    /**
     * The development version
     *
     * \since QGIS 3.12
     */
    static QString devVersion();

    // Enumerations
    //

    /**
     * \brief Level for messages
     * This will be used both for message log and message bar in application.
     */
    enum MessageLevel
    {
      Info = 0,
      Warning = 1,
      Critical = 2,
      Success = 3,
      None = 4
    };

    /**
     * Raster data types.
     *  This is modified and extended copy of GDALDataType.
     */
    enum DataType
    {
      UnknownDataType = 0, //!< Unknown or unspecified type
      Byte = 1, //!< Eight bit unsigned integer (quint8)
      UInt16 = 2, //!< Sixteen bit unsigned integer (quint16)
      Int16 = 3, //!< Sixteen bit signed integer (qint16)
      UInt32 = 4, //!< Thirty two bit unsigned integer (quint32)
      Int32 = 5, //!< Thirty two bit signed integer (qint32)
      Float32 = 6, //!< Thirty two bit floating point (float)
      Float64 = 7, //!< Sixty four bit floating point (double)
      CInt16 = 8, //!< Complex Int16
      CInt32 = 9, //!< Complex Int32
      CFloat32 = 10, //!< Complex Float32
      CFloat64 = 11, //!< Complex Float64
      ARGB32 = 12, //!< Color, alpha, red, green, blue, 4 bytes the same as QImage::Format_ARGB32
      ARGB32_Premultiplied = 13 //!< Color, alpha, red, green, blue, 4 bytes  the same as QImage::Format_ARGB32_Premultiplied
    };
    Q_ENUM( DataType )

    /**
     * Authorisation to run Python Macros
     * \since QGIS 3.10
     */
    enum PythonMacroMode
    {
      Never = 0, //!< Macros are never run
      Ask = 1, //!< User is prompt before running
      SessionOnly = 2, //!< Only during this session
      Always = 3, //!< Macros are always run
      NotForThisSession, //!< Macros will not be run for this session
    };
    Q_ENUM( PythonMacroMode )

    /**
     * Identify search radius in mm
     * \since QGIS 2.3
     */
    static const double DEFAULT_SEARCH_RADIUS_MM;

    //! Default threshold between map coordinates and device coordinates for map2pixel simplification
    static const float DEFAULT_MAPTOPIXEL_THRESHOLD;

    /**
     * Default highlight color.  The transparency is expected to only be applied to polygon
     * fill. Lines and outlines are rendered opaque.
     *
     *  \since QGIS 2.3
     */
    static const QColor DEFAULT_HIGHLIGHT_COLOR;

    /**
     * Default highlight buffer in mm.
     *  \since QGIS 2.3
     */
    static const double DEFAULT_HIGHLIGHT_BUFFER_MM;

    /**
     * Default highlight line/stroke minimum width in mm.
     * \since QGIS 2.3
     */
    static const double DEFAULT_HIGHLIGHT_MIN_WIDTH_MM;

    /**
     * Fudge factor used to compare two scales. The code is often going from scale to scale
     *  denominator. So it looses precision and, when a limit is inclusive, can lead to errors.
     *  To avoid that, use this factor instead of using <= or >=.
     * \since QGIS 2.15
     */
    static const double SCALE_PRECISION;

    /**
     * Default Z coordinate value for 2.5d geometry
     *  This value have to be assigned to the Z coordinate for the new 2.5d geometry vertex.
     *  \since QGIS 3.0
     */
    static const double DEFAULT_Z_COORDINATE;

    /**
     * UI scaling factor. This should be applied to all widget sizes obtained from font metrics,
     * to account for differences in the default font sizes across different platforms.
     *  \since QGIS 3.0
     */
    static const double UI_SCALE_FACTOR;

    /**
     * Default snapping distance tolerance.
     *  \since QGIS 3.0
     */
    static const double DEFAULT_SNAP_TOLERANCE;

    /**
     * Default snapping distance units.
     *  \since QGIS 3.0
     */
    static const QgsTolerance::UnitType DEFAULT_SNAP_UNITS;

    /**
     * A string with default project scales.
     *
     * \since QGIS 3.12
     */
    static QString defaultProjectScales();
};

// hack to workaround warnings when casting void pointers
// retrieved from QLibrary::resolve to function pointers.
// It's assumed that this works on all systems supporting
// QLibrary
#define cast_to_fptr(f) f


/**
 * \ingroup core
 * \brief RAII signal blocking class. Used for temporarily blocking signals from a QObject
 * for the lifetime of QgsSignalBlocker object.
 * \see whileBlocking()
 * \note not available in Python bindings
 * \since QGIS 2.16
 */
// based on Boojum's code from http://stackoverflow.com/questions/3556687/prevent-firing-signals-in-qt
template<class Object> class QgsSignalBlocker SIP_SKIP SIP_SKIP // clazy:exclude=rule-of-three
{
  public:

    /**
     * Constructor for QgsSignalBlocker
     * \param object QObject to block signals from
     */
    explicit QgsSignalBlocker( Object *object )
      : mObject( object )
      , mPreviousState( object->blockSignals( true ) )
    {}

    ~QgsSignalBlocker()
    {
      mObject->blockSignals( mPreviousState );
    }

    //! Returns pointer to blocked QObject
    Object *operator->() { return mObject; }

  private:

    Object *mObject = nullptr;
    bool mPreviousState;

};

/**
 * Temporarily blocks signals from a QObject while calling a single method from the object.
 *
 * Usage:
 *   whileBlocking( checkBox )->setChecked( true );
 *   whileBlocking( spinBox )->setValue( 50 );
 *
 * No signals will be emitted when calling these methods.
 *
 * \see QgsSignalBlocker
 * \note not available in Python bindings
 * \since QGIS 2.16
 */
// based on Boojum's code from http://stackoverflow.com/questions/3556687/prevent-firing-signals-in-qt
template<class Object> inline QgsSignalBlocker<Object> whileBlocking( Object *object ) SIP_SKIP SIP_SKIP
{
  return QgsSignalBlocker<Object>( object );
}

//! Hash for QVariant
CORE_EXPORT uint qHash( const QVariant &variant );

/**
 * Returns a string representation of a double
 * \param a double value
 * \param precision number of decimal places to retain
 */
inline QString qgsDoubleToString( double a, int precision = 17 )
{
  QString str = QString::number( a, 'f', precision );
  if ( precision )
  {
    if ( str.contains( QLatin1Char( '.' ) ) )
    {
      // remove ending 0s
      int idx = str.length() - 1;
      while ( str.at( idx ) == '0' && idx > 1 )
      {
        idx--;
      }
      if ( idx < str.length() - 1 )
        str.truncate( str.at( idx ) == '.' ? idx : idx + 1 );
    }
  }
  // avoid printing -0
  // see https://bugreports.qt.io/browse/QTBUG-71439
  if ( str == QLatin1String( "-0" ) )
  {
    return QLatin1String( "0" );
  }
  return str;
}

/**
 * Compare two doubles (but allow some difference)
 * \param a first double
 * \param b second double
 * \param epsilon maximum difference allowable between doubles
 */
inline bool qgsDoubleNear( double a, double b, double epsilon = 4 * std::numeric_limits<double>::epsilon() )
{
  if ( std::isnan( a ) || std::isnan( b ) )
    return std::isnan( a ) && std::isnan( b ) ;

  const double diff = a - b;
  return diff > -epsilon && diff <= epsilon;
}

/**
 * Compare two floats (but allow some difference)
 * \param a first float
 * \param b second float
 * \param epsilon maximum difference allowable between floats
 */
inline bool qgsFloatNear( float a, float b, float epsilon = 4 * FLT_EPSILON )
{
  if ( std::isnan( a ) || std::isnan( b ) )
    return std::isnan( a ) && std::isnan( b ) ;

  const float diff = a - b;
  return diff > -epsilon && diff <= epsilon;
}

//! Compare two doubles using specified number of significant digits
inline bool qgsDoubleNearSig( double a, double b, int significantDigits = 10 )
{
  if ( std::isnan( a ) || std::isnan( b ) )
    return std::isnan( a ) && std::isnan( b ) ;

  // The most simple would be to print numbers as %.xe and compare as strings
  // but that is probably too costly
  // Then the fastest would be to set some bits directly, but little/big endian
  // has to be considered (maybe TODO)
  // Is there a better way?
  int aexp, bexp;
  double ar = std::frexp( a, &aexp );
  double br = std::frexp( b, &bexp );

  return aexp == bexp &&
         std::round( ar * std::pow( 10.0, significantDigits ) ) == std::round( br * std::pow( 10.0, significantDigits ) );
}

/**
 * Returns a double \a number, rounded (as close as possible) to the specified number of \a places.
 *
 * \since QGIS 3.0
 */
inline double qgsRound( double number, int places )
{
  double m = ( number < 0.0 ) ? -1.0 : 1.0;
  double scaleFactor = std::pow( 10.0, places );
  return ( std::round( number * m * scaleFactor ) / scaleFactor ) * m;
}


#ifndef SIP_RUN

///@cond PRIVATE

/**
 * Contains "polyfills" for backporting c++ features from standards > c++11 and Qt global methods
 * added later than our minimum version.
 *
 * To be removed when minimum c++ or Qt build requirement includes the std implementation
 * for these features.
 *
 * \note not available in Python bindings.
 */
namespace qgis
{

  template<class T>
  QSet<T> listToSet( const QList<T> &list )
  {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    return list.toSet();
#else
    return QSet<T>( list.begin(), list.end() );
#endif
  }

  template<class T>
  QList<T> setToList( const QSet<T> &set )
  {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    return set.toList();
#else
    return QList<T>( set.begin(), set.end() );
#endif
  }
}
///@endcond
#endif

/**
 * Returns a map of all enum entries.
 * The map has the enum values (int) as keys and the enum keys (QString) as values.
 * The enum must have been declared using Q_ENUM or Q_FLAG.
 */
template<class T> const QMap<T, QString> qgsEnumMap() SIP_SKIP
{
  QMetaEnum metaEnum = QMetaEnum::fromType<T>();
  Q_ASSERT( metaEnum.isValid() );
  QMap<T, QString> enumMap;
  for ( int idx = 0; idx < metaEnum.keyCount(); ++idx )
  {
    const char *enumKey = metaEnum.key( idx );
    enumMap.insert( static_cast<T>( metaEnum.keyToValue( enumKey ) ), QString( enumKey ) );
  }
  return enumMap;
}

/**
 * Returns the value for the given key of an enum.
 * \since QGIS 3.6
 */
template<class T> QString qgsEnumValueToKey( const T &value ) SIP_SKIP
{
  QMetaEnum metaEnum = QMetaEnum::fromType<T>();
  Q_ASSERT( metaEnum.isValid() );
  return QString::fromUtf8( metaEnum.valueToKey( static_cast<int>( value ) ) );
}

/**
 * Returns the value corresponding to the given \a key of an enum.
 * If the key is invalid, it will return the \a defaultValue.
 * If \a tryValueAsKey is TRUE, it will try to convert the string key to an enum value
 * \since QGIS 3.6
 */
template<class T> T qgsEnumKeyToValue( const QString &key, const T &defaultValue, bool tryValueAsKey = true ) SIP_SKIP
{
  QMetaEnum metaEnum = QMetaEnum::fromType<T>();
  Q_ASSERT( metaEnum.isValid() );
  bool ok = false;
  T v = static_cast<T>( metaEnum.keyToValue( key.toUtf8().data(), &ok ) );
  if ( ok )
  {
    return v;
  }
  else
  {
    // if conversion has failed, try with conversion from int value
    if ( tryValueAsKey )
    {
      bool canConvert = false;
      int intValue = key.toInt( &canConvert );
      if ( canConvert && metaEnum.valueToKey( intValue ) )
      {
        return static_cast<T>( intValue );
      }
    }
  }
  return defaultValue;
}

/**
 * Returns the value for the given keys of a flag.
 * \since QGIS 3.16
 */
template<class T> QString qgsFlagValueToKeys( const T &value ) SIP_SKIP
{
  QMetaEnum metaEnum = QMetaEnum::fromType<T>();
  Q_ASSERT( metaEnum.isValid() );
  return QString::fromUtf8( metaEnum.valueToKeys( static_cast<int>( value ) ) );
}

/**
 * Returns the value corresponding to the given \a keys of a flag.
 * If the keys are invalid, it will return the \a defaultValue.
 * \since QGIS 3.16
 */
template<class T> T qgsFlagKeysToValue( const QString &keys, const T &defaultValue ) SIP_SKIP
{
  QMetaEnum metaEnum = QMetaEnum::fromType<T>();
  Q_ASSERT( metaEnum.isValid() );
  bool ok = false;
  T v = static_cast<T>( metaEnum.keysToValue( keys.toUtf8().constData(), &ok ) );
  if ( ok )
    return v;
  else
    return defaultValue;
}


/**
 * Converts a string to a double in a permissive way, e.g., allowing for incorrect
 * numbers of digits between thousand separators
 * \param string string to convert
 * \param ok will be set to TRUE if conversion was successful
 * \returns string converted to double if possible
 * \see permissiveToInt
 * \since QGIS 2.9
 */
CORE_EXPORT double qgsPermissiveToDouble( QString string, bool &ok );

/**
 * Converts a string to an integer in a permissive way, e.g., allowing for incorrect
 * numbers of digits between thousand separators
 * \param string string to convert
 * \param ok will be set to TRUE if conversion was successful
 * \returns string converted to int if possible
 * \see permissiveToDouble
 * \since QGIS 2.9
 */
CORE_EXPORT int qgsPermissiveToInt( QString string, bool &ok );

/**
 * Converts a string to an qlonglong in a permissive way, e.g., allowing for incorrect
 * numbers of digits between thousand separators
 * \param string string to convert
 * \param ok will be set to TRUE if conversion was successful
 * \returns string converted to int if possible
 * \see permissiveToInt
 * \since QGIS 3.4
 */
CORE_EXPORT qlonglong qgsPermissiveToLongLong( QString string, bool &ok );

/**
 * Compares two QVariant values and returns whether the first is less than the second.
 * Useful for sorting lists of variants, correctly handling sorting of the various
 * QVariant data types (such as strings, numeric values, dates and times)
 *
 * Invalid < NULL < Values
 *
 * \see qgsVariantGreaterThan()
 */
CORE_EXPORT bool qgsVariantLessThan( const QVariant &lhs, const QVariant &rhs );

/**
 * Compares two QVariant values and returns whether they are equal, two NULL values are
 * always treated as equal and 0 is not treated as equal with NULL
 *
 * \param lhs first value
 * \param rhs second value
 * \return TRUE if values are equal
 */
CORE_EXPORT bool qgsVariantEqual( const QVariant &lhs, const QVariant &rhs );

/**
 * Compares two QVariant values and returns whether the first is greater than the second.
 * Useful for sorting lists of variants, correctly handling sorting of the various
 * QVariant data types (such as strings, numeric values, dates and times)
 * \see qgsVariantLessThan()
 */
CORE_EXPORT bool qgsVariantGreaterThan( const QVariant &lhs, const QVariant &rhs );

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

/**
 * Compares two QVariant values and returns whether the first is greater than the second.
 * Useful for sorting lists of variants, correctly handling sorting of the various
 * QVariant data types (such as strings, numeric values, dates and times)
 * \see qgsVariantLessThan()
 */
inline bool operator> ( const QVariant &v1, const QVariant &v2 )
{
  return qgsVariantGreaterThan( v1, v2 );
}

/**
 * Compares two QVariant values and returns whether the first is less than the second.
 * Useful for sorting lists of variants, correctly handling sorting of the various
 * QVariant data types (such as strings, numeric values, dates and times)
 *
 * Invalid < NULL < Values
 *
 * \see qgsVariantGreaterThan()
 */
inline bool operator< ( const QVariant &v1, const QVariant &v2 )
{
  return qgsVariantLessThan( v1, v2 );
}
#endif


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

/**
 * Compares two QVariantList values and returns whether the first is less than the second.
 */
template<> CORE_EXPORT bool qMapLessThanKey<QVariantList>( const QVariantList &key1, const QVariantList &key2 ) SIP_SKIP;
#endif

CORE_EXPORT QString qgsVsiPrefix( const QString &path );

/**
 * Allocates size bytes and returns a pointer to the allocated  memory.
 * Works like C malloc() but prints debug message by QgsLogger if allocation fails.
 * \param size size in bytes
 */
void CORE_EXPORT *qgsMalloc( size_t size ) SIP_SKIP;

/**
 * Allocates  memory for an array of nmemb elements of size bytes each and returns
 * a pointer to the allocated memory. Works like C calloc() but prints debug message
 * by QgsLogger if allocation fails.
 * \param nmemb number of elements
 * \param size size of element in bytes
 */
void CORE_EXPORT *qgsCalloc( size_t nmemb, size_t size ) SIP_SKIP;

/**
 * Frees the memory space  pointed  to  by  ptr. Works like C free().
 * \param ptr pointer to memory space
 */
void CORE_EXPORT qgsFree( void *ptr ) SIP_SKIP;

#ifndef SIP_RUN

#ifdef _MSC_VER
#define CONSTLATIN1STRING inline const QLatin1String
#else
#define CONSTLATIN1STRING constexpr QLatin1String
#endif

/**
* Wkt string that represents a geographic coord sys
* \since QGIS GEOWkt
*/
CONSTLATIN1STRING geoWkt()
{
  return QLatin1String(
           R"""(GEOGCRS["WGS 84",DATUM["World Geodetic System 1984",ELLIPSOID["WGS 84",6378137,298.257223563,LENGTHUNIT["metre",1]]],PRIMEM["Greenwich",0,ANGLEUNIT["degree",0.0174532925199433]],CS[ellipsoidal,2],AXIS["geodetic latitude (Lat)",north,ORDER[1],ANGLEUNIT["degree",0.0174532925199433]],AXIS["geodetic longitude (Lon)",east,ORDER[2],ANGLEUNIT["degree",0.0174532925199433]],USAGE[SCOPE["unknown"],AREA["World"],BBOX[-90,-180,90,180]],ID["EPSG",4326]] )"""
         );
}

//! PROJ4 string that represents a geographic coord sys
CONSTLATIN1STRING geoProj4()
{
  return QLatin1String( "+proj=longlat +datum=WGS84 +no_defs" );
}

//! Geographic coord sys from EPSG authority
CONSTLATIN1STRING geoEpsgCrsAuthId()
{
  return QLatin1String( "EPSG:4326" );
}

//! Constant that holds the string representation for "No ellips/No CRS"
CONSTLATIN1STRING geoNone()
{
  return QLatin1String( "NONE" );
}

///@cond PRIVATE

//! Delay between the scheduling of 2 preview jobs
const int PREVIEW_JOB_DELAY_MS = 250;

//! Maximum rendering time for a layer of a preview job
const int MAXIMUM_LAYER_PREVIEW_TIME_MS = 250;

///@endcond

#endif

//! Magic number for a geographic coord sys in POSTGIS SRID
const long GEOSRID = 4326;

//! Magic number for a geographic coord sys in QGIS srs.db tbl_srs.srs_id
const long GEOCRS_ID = 3452;

//! Magic number for a geographic coord sys in EpsgCrsId ID format
const long GEO_EPSG_CRS_ID = 4326;

/**
 * Magick number that determines whether a projection crsid is a system (srs.db)
 * or user (~/.qgis.qgis.db) defined projection.
*/
const int USER_CRS_START_ID = 100000;

//
// Constants for point symbols
//

//! Magic number that determines the default point size for point symbols
const double DEFAULT_POINT_SIZE = 2.0;
const double DEFAULT_LINE_WIDTH = 0.26;

//! Default snapping tolerance for segments
const double DEFAULT_SEGMENT_EPSILON = 1e-8;

typedef QMap<QString, QString> QgsStringMap SIP_SKIP;

/**
 * Qgssize is used instead of size_t, because size_t is stdlib type, unknown
 *  by SIP, and it would be hard to define size_t correctly in SIP.
 *  Currently used "unsigned long long" was introduced in C++11 (2011)
 *  but it was supported already before C++11 on common platforms.
 *  "unsigned long long int" gives syntax error in SIP.
 *  KEEP IN SYNC WITH qgssize defined in SIP!
*/
typedef unsigned long long qgssize;

#ifndef SIP_RUN
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) || defined(__clang__)

#define Q_NOWARN_DEPRECATED_PUSH \
  _Pragma("GCC diagnostic push") \
  _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"");
#define Q_NOWARN_DEPRECATED_POP \
  _Pragma("GCC diagnostic pop");
#define Q_NOWARN_UNREACHABLE_PUSH
#define Q_NOWARN_UNREACHABLE_POP

#elif defined(_MSC_VER)

#define Q_NOWARN_DEPRECATED_PUSH \
  __pragma(warning(push)) \
  __pragma(warning(disable:4996))
#define Q_NOWARN_DEPRECATED_POP \
  __pragma(warning(pop))
#define Q_NOWARN_UNREACHABLE_PUSH \
  __pragma(warning(push)) \
  __pragma(warning(disable:4702))
#define Q_NOWARN_UNREACHABLE_POP \
  __pragma(warning(pop))

#else

#define Q_NOWARN_DEPRECATED_PUSH
#define Q_NOWARN_DEPRECATED_POP
#define Q_NOWARN_UNREACHABLE_PUSH
#define Q_NOWARN_UNREACHABLE_POP

#endif
#endif

#ifndef QGISEXTERN
#ifdef Q_OS_WIN
#  define QGISEXTERN extern "C" __declspec( dllexport )
#  ifdef _MSC_VER
// do not warn about C bindings returning QString
#    pragma warning(disable:4190)
#  endif
#else
#  if defined(__GNUC__) || defined(__clang__)
#    define QGISEXTERN extern "C" __attribute__ ((visibility ("default")))
#  else
#    define QGISEXTERN extern "C"
#  endif
#endif
#endif
#endif

#if __cplusplus >= 201500
#define FALLTHROUGH [[fallthrough]];
#elif defined(__clang__)
#define FALLTHROUGH [[clang::fallthrough]];
#elif defined(__GNUC__) && __GNUC__ >= 7
#define FALLTHROUGH [[gnu::fallthrough]];
#else
#define FALLTHROUGH
#endif

// see https://infektor.net/posts/2017-01-19-using-cpp17-attributes-today.html#using-the-nodiscard-attribute
#if __cplusplus >= 201703L
#define NODISCARD [[nodiscard]]
#elif defined(__clang__)
#define NODISCARD [[nodiscard]]
#elif defined(_MSC_VER)
#define NODISCARD // no support
#elif defined(__has_cpp_attribute)
#if __has_cpp_attribute(nodiscard)
#define NODISCARD [[nodiscard]]
#elif __has_cpp_attribute(gnu::warn_unused_result)
#define NODISCARD [[gnu::warn_unused_result]]
#else
#define NODISCARD Q_REQUIRED_RESULT
#endif
#else
#define NODISCARD Q_REQUIRED_RESULT
#endif

#if __cplusplus >= 201703L
#define MAYBE_UNUSED [[maybe_unused]]
#elif defined(__clang__)
#define MAYBE_UNUSED [[maybe_unused]]
#elif defined(_MSC_VER)
#define MAYBE_UNUSED // no support
#elif defined(__has_cpp_attribute)
#if __has_cpp_attribute(gnu::unused)
#define MAYBE_UNUSED [[gnu::unused]]
#else
#define MAYBE_UNUSED
#endif
#else
#define MAYBE_UNUSED
#endif

#ifndef FINAL
#define FINAL final
#endif

#ifdef SIP_RUN

/**
 * Wkt string that represents a geographic coord sys
 * \since QGIS GEOWkt
 */
QString CORE_EXPORT geoWkt();

//! PROJ4 string that represents a geographic coord sys
QString CORE_EXPORT geoProj4();

//! Geographic coord sys from EPSG authority
QString CORE_EXPORT geoEpsgCrsAuthId();

//! Constant that holds the string representation for "No ellips/No CRS"
QString CORE_EXPORT geoNone();

#endif
