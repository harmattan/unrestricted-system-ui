/**
 * @brief DBus constants for profile daemon
 *
 * @file profile_dbus.h
 *
 * The Constants used for communication with profile daemon over dbus.
 *
 * <p>
 * Copyright (C) 2008 Nokia. All rights reserved.
 *
 * @author Simo Piiroinen <simo.piiroinen@nokia.com>
 */

/* ========================================================================= *
 * File: profile_dbus.h
 *
 * Copyright (C) 2008 Nokia. All rights reserved.
 *
 * Author: Simo Piiroinen <simo.piiroinen@nokia.com>
 *
 * -------------------------------------------------------------------------
 *
 * History:
 *
 * 20-May-2008 Simo Piiroinen
 * - added doxygen comments
 *
 * 15-May-2008 Simo Piiroinen
 * - initial version
 * ========================================================================= */

#ifndef PROFILE_DBUS_H_
# define PROFILE_DBUS_H_

/** @name DBus Daemon
 */

/*@{*/

/**
 * Profile daemon DBus service.
 **/
# define PROFILED_SERVICE "com.nokia.profiled"

/**
 * Profile daemon DBus object path.
 **/
# define PROFILED_PATH "/com/nokia/profiled"

/**
 * Profile daemon DBus method call and signal interface.
 **/
# define PROFILED_INTERFACE "com.nokia.profiled"

/*@}*/

/** @name DBus Methods
 */

/*@{*/

/**
 * Get active profile name.
 *
 * @param n/a
 * @returns profile : STRING
 **/
# define PROFILED_GET_PROFILE  "get_profile"

/**
 * Check existance of profile name.
 *
 * @param profile : STRING
 *
 * @returns exists : BOOLEAN
 **/
# define PROFILED_HAS_PROFILE  "has_profile"

/**
 * Set active profile name.
 *
 * @param profile : STRING
 *
 * @returns success : BOOLEAN
 **/
# define PROFILED_SET_PROFILE  "set_profile"

/**
 * Get available profiles.
 *
 * @param n/a
 *
 * @returns profiles : ARRAY of STRING
 **/
# define PROFILED_GET_PROFILES "get_profiles"

/**
 * Get available keys.
 *
 * @param n/a
 *
 * @returns keys : ARRAY of STRING
 **/
# define PROFILED_GET_KEYS     "get_keys"

/**
 * Get profile value.
 *
 * @param profile : STRING
 * @param key     : STRING
 *
 * @returns value : STRING
 **/
# define PROFILED_GET_VALUE    "get_value"

/**
 * Check existance of value.
 *
 * @param key : STRING
 *
 * @returns exists : BOOLEAN
 **/
# define PROFILED_HAS_VALUE    "has_value"

/**
 * Check if value can be modified.
 *
 * @param key : STRING
 *
 * @returns writable : BOOLEAN
 **/
# define PROFILED_IS_WRITABLE  "is_writable"

/**
 * Set profile value.
 *
 * @param   profile : STRING
 * @param   key     : STRING
 * @param   val     : STRING
 *
 * @returns success : BOOLEAN
 **/
# define PROFILED_SET_VALUE    "set_value"

/**
 * Get type of profile value.
 *
 * @param   profile : STRING
 * @param   key     : STRING
 *
 * @returns type    : STRING
 **/
# define PROFILED_GET_TYPE     "get_type"

/**
 * Get all profile values.
 *
 * @param   profile : STRING
 *
 * @returns values : ARRAY of STRUCT
 *         <br> key  : STRING
 *         <br> val  : STRING
 *         <br> type : STRING
 **/
# define PROFILED_GET_VALUES   "get_values"

/*@}*/

/** @name DBus Signals
 */

/*@{*/

/**
 * Signal emitted after changes to profile data
 *
 * @param changed : BOOLEAN
 * @param active  : BOOLEAN
 * @param profile : STRING
 * @param values  : ARRAY of STRUCT
 *         <br> key  : STRING
 *         <br> val  : STRING
 *         <br> type : STRING
 **/
# define PROFILED_CHANGED      "profile_changed"

/*@}*/

#endif
