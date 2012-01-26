
/* Copyright (c) 2009-2011, Stefan Eilemann <eile@equalizergraphics.com> 
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EQ_STATISTIC_H
#define EQ_STATISTIC_H

#include <eq/client/api.h>
#include <eq/client/types.h>

#include <iostream>

namespace eq
{
    /**
     * A statistics event.
     *
     * Statistics are events generated by various Equalizer entities, typically
     * using StatisticSampler. They are used by the server for load-balancing
     * and to render the statistics overlay.
     * @sa Channel::drawStatistics
     */
    struct Statistic
    {
        /** The type of the statistics event. */
        enum Type // Also update string and color table in statistic.cpp
        {
            NONE = 0,
            CHANNEL_CLEAR, //!< Sampling of Channel::frameClear
            CHANNEL_DRAW, //!< Sampling of Channel::frameDraw
            CHANNEL_DRAW_FINISH, //!< Sampling of Channel::frameDrawFinish
            CHANNEL_ASSEMBLE, //!< Sampling of Channel::frameAssemble
            CHANNEL_FRAME_WAIT_READY, //!< Sampling of Frame::waitReady
            CHANNEL_READBACK, //!< Sampling of Channel::frameReadback
            CHANNEL_VIEW_FINISH, //!< Sampling of Channel::frameViewFinish
            CHANNEL_FRAME_TRANSMIT, //!< Sampling of frame transmission
            CHANNEL_FRAME_COMPRESS, //!< Sampling of frame compression
            /** Sampling of waiting for a send token from the receiver */
            CHANNEL_FRAME_WAIT_SENDTOKEN,
            WINDOW_FINISH, //!< Sampling of Window::finish before a swap barrier
            /** Sampling of throttling of framerate_equalizer */
            WINDOW_THROTTLE_FRAMERATE,
            WINDOW_SWAP_BARRIER, //!< Sampling of swap barrier block
            WINDOW_SWAP, //!< Sampling of Window::swapBuffers
            WINDOW_FPS, //!< Framerate sampling
            PIPE_IDLE, //!< Pipe thread idle ratio
            NODE_FRAME_DECOMPRESS, //!< Sampling of frame decompression
            CONFIG_START_FRAME, //!< Sampling of Config::startFrame
            CONFIG_FINISH_FRAME, //!< Sampling of Config::finishFrame
            /** Sampling of synchronization time during Config::finishFrame */
            CONFIG_WAIT_FINISH_FRAME,
            ALL          // must be last
        };

        Type type; //!< The type of statistic
        uint32_t frameNumber; //!< The frame during when the sampling happened
        uint32_t task; //!< @internal
        uint32_t plugins[2]; //!< color,depth plugins (readback, compression)
        float ratio; //!< compression ratio (transfer, compression)
        
        union
        {
            int64_t  startTime; //!< Absolute start time of the operation
            int64_t  idleTime;  //!< Absolute idle time of PIPE_IDLE
            float    currentFPS; //!< FPS of last frame (WINDOW_FPS)
        };
        union
        {
            int64_t  endTime;    //!< Absolute end time of the operation
            int64_t  totalTime;  //!< Total time of a pipe frame (PIPE_IDLE)
            float    averageFPS; //!< Weighted sum averaging of FPS (WINDOW_FPS)
        };

        char resourceName[32]; //!< A non-unique name of the originator

        /** Translate the Type to a string representation. @version 1.0 */
        static const std::string& getName( const Type type );
        /** Translate the Type to a color value. @version 1.0 */
        static const Vector3f& getColor( const Type type );
    };

    /** Output the statistic type to an std::ostream. @version 1.0 */
    EQ_API std::ostream& operator << ( std::ostream&, const Statistic::Type& );

    /** Output the statistic to an std::ostream. @version 1.0 */
    EQ_API std::ostream& operator << ( std::ostream&, const Statistic& );
}

#endif // EQ_STATISTIC_H
