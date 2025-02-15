SET(AQSIS_OSACOMPILE_SEARCHPATH)
IF(APPLE)
	FIND_PROGRAM(AQSIS_OSACOMPILE_EXECUTABLE
		osacompile
		PATHS ${AQSIS_OSACOMPILE_SEARCHPATH}
		DOC "Location of the osacompile executable"
	)

	STRING(COMPARE NOTEQUAL ${AQSIS_OSACOMPILE_EXECUTABLE} "AQSIS_OSACOMPILE_EXECUTABLE-NOTFOUND" AQSIS_OSACOMPILE_EXECUTABLE_FOUND)
ENDIF(APPLE)