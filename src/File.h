/*=============================================================================
Blobby Volley 2
Copyright (C) 2006 Jonathan Sieber (jonathan_sieber@yahoo.de)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=============================================================================*/

#pragma once

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

/**
	\class File
	\brief convenience wrapper around PHYSFS_file
	\details This class provides object oriented access to physfs files. Furthermore,
			it automatically closes opened files on destruction to prevent leaks. 
			For now, it is defined as noncopyable, because we don't have implemented
			any copying behaviour yet.
	\todo	check the return parameters of the PHYSFS functions, and do error handling accordingly
	\todo 	write tests for this class.
*/
class File : boost::noncopyable 
{
	public:
		enum OpenMode {
			OPEN_READ,
			OPEN_WRITE
		};
	
		/// \brief default ctor
		/// \details File has to be opended with open()
		/// \throw nothing
		explicit File();
		
		/// \brief constructor which opens a file.
		/// \param filename File to be opened
		/// \param mode Should this file be opened in reading or in writing mode.
		/// \throw FileLoadException, if the file could not be loaded
		File(const std::string& filename, OpenMode mode);
		
		/// \brief opens a file.
		/// \param filename File to be opened
		/// \param mode Should this file be opened in reading or in writing mode.
		/// \throw FileLoadException, if the file could not be loaded
		/// \pre No file is currently opened.
		void open(const std::string& filename, OpenMode mode);
		
		/// \brief closes the file
		/// \details This function does nothing when the no file is opened.
		///			This behaviour is necessary as this function is called in the destructor.
		/// \throw nothing
		void close();
		
		/// destructor, closes the file (if any open)
		/// \sa close()
		/// \throw nothing
		~File();
		
		/// \brief gets the PHYSFS_file as void*
		/// \details We don't return a PHYSFS_file* here, as this type is a typedef so it cannot
		///			 be forward declared, which would require us to include <physfs.h> here. 
		/// \throw nothing
		void* getPHYSFS_file();
		
		// ------------------------------------
		// information querying interface
		// ------------------------------------
		
		/// \brief checks if a file is opened
		/// \throw nothing
		bool is_open() const;
		
		/// \todo check if these return types fit!
		/// gets the length of the currently opened file.
		/// don't call it if there is no opened file!
		/// \throw nothing
		uint32_t length() const;
		
		/// gets the current reading position.
		/// don't call if no file opened
		/// \throw nothing
		uint32_t tell() const;
		
		// ------------------------------------
		//  reading interface
		// ------------------------------------
		/// reads bytes into a buffer
		/// \param target buffer to read into
		/// \param num_of_bytes number of bytes to read
		/// \throw nothing (for now)
		uint32_t readRawBytes( char* target, std::size_t num_of_bytes );
		
		
		/// reads bytes and returns a safe-pointed buffer
		/// the buffer is allocated by this function and has a size of \p num_of_bytes
		/// \param num_of_bytes Number of bytes to read; size of buffer
		/// \throw nothing
		boost::shared_array<char> readRawBytes( std::size_t num_of_bytes );
		
		// ------------------------------------
		//  writing interface
		// ------------------------------------
		
		/// \brief writes a std::string
		/// \details writes the content of the string to the file.
		/// 		does not write a null-termination character
		/// \throw nothing
		void write(const std::string& data);
		
		/// \brief writes a sequence of characters
		/// \details writes \p length characters from \p data to the file
		/// \throw nothing
		void write(const char* data, std::size_t length);
		
	private:
		/// we use void* instead of PHYSFS_file here, because we can't forward declare it
		///	as it is a typedef.
		void* handle;
};