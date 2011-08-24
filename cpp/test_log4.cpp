
// TestLog4CPP.cpp : A small exerciser for log4cpp

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/BasicLayout.hh>
#include <unistd.h>

int main(int argc, char** argv) {

        // 1 instantiate an appender object that 
        // will append to a log file
	log4cpp::Appender* app = new 
              log4cpp::FileAppender("FileAppender",
              "testlog4cpp.log");

        // 2. Instantiate a layout object
	// Two layouts come already available in log4cpp
	// unless you create your own.
	// BasicLayout includes a time stamp
        log4cpp::Layout* layout = 
        new log4cpp::BasicLayout();

	// 3. attach the layout object to the 
	// appender object
	app->setLayout(layout);

	// 4. Instantiate the category object
	// you may extract the root category, but it is
	// usually more practical to directly instance
	// a child category
//     log4cpp::Category main_cat = log4cpp::Category::getInstance("main_cat");
    log4cpp::Category& main_cat = log4cpp::Category::getInstance("main_cat");

	// 5. Step 1 
	// an Appender when added to a category becomes
	// an additional output destination unless 
	// Additivity is set to false when it is false,
	// the appender added to the category replaces
	// all previously existing appenders
    main_cat.setAdditivity(false);

	// 5. Step 2
    // this appender becomes the only one
	main_cat.setAppender(app);

	// 6. Set up the priority for the category
    // and is given INFO priority
	// attempts to log DEBUG messages will fail
	main_cat.setPriority(log4cpp::Priority::INFO);

	// so we log some examples
	main_cat.info("This is some info");
    sleep(1);
	main_cat.debug("This debug message will fail to write");
    sleep(1);
	main_cat.alert("All hands abandon ship");
    sleep(1);

	// you can log by using a log() method with 
	// a priority
	main_cat.log(log4cpp::Priority::WARN, "This will "
                 "be a logged warning");
    sleep(1);
	// gives you some programmatic control over 
	// priority levels
	log4cpp::Priority::PriorityLevel priority;
	bool this_is_critical = true;
	if(this_is_critical)
		priority = log4cpp::Priority::CRIT;
	else
		priority = log4cpp::Priority::DEBUG;

	// this would not be logged if priority 
	// == DEBUG, because the category priority is 
	// set to INFO
	main_cat.log(priority,"Importance depends on "
                 "context");
	
	// You may also log by using stream style
	// operations on 
	main_cat.critStream() << "This will show up "
             " << as " << 1 << " critical message" 
	<< log4cpp::CategoryStream::ENDLINE;
        main_cat.emergStream() << "This will show up as " 
		<< 1 << " emergency message" <<       
                log4cpp::CategoryStream::ENDLINE;

	// Stream operations can be used directly 
        // with the main object, but are 
        // preceded by the severity level
	main_cat << log4cpp::Priority::ERROR 
              << "And this will be an error"  
              << log4cpp::CategoryStream::ENDLINE;

	// This illustrates a small bug in version 
	// 2.5 of log4cpp
	main_cat.debug("debug"); // this is correctly 
				 // skipped
	main_cat.info("info");
	main_cat.notice("notice");
	main_cat.warn("warn");
	main_cat.error("error");
	main_cat.crit("crit");	// this prints ALERT 
				// main_cat : crit	
	main_cat.alert("alert");// this prints PANIC 
				// main_cat : alert
	main_cat.emerg("emerg");// this prints UNKOWN 
				// main_cat : emerg


	main_cat.debug("Shutting down");// this will 
					// be skipped

	// clean up and flush all appenders
	log4cpp::Category::shutdown();
	return 0;
}
