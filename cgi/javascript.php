<?php
            # set appropriate content type - to tell the browser we're returning Javascript
            header( 'Content-Type: text/javascript' );         

            # start the session
            session_start();           

            # determine the current time
            $time                = date( 'g:i a' );           

            # determine how many times the user has called the function, then increment it
            $js_count          = ( ++$_SESSION['js_count'] );           

            # based on the count, set the appropriate display value
            if ( $js_count == 1 ) {
                        $inner_html       = "This is your first time asking. It is $time.";
            } else if ( $js_count <= 3 ) {
                        $inner_html       = "You have asked me $js_count times now. It is $time.";
            } else {
                        $inner_html       = "You have already asked me $js_count times. Shut up!";
            } # END big-nasty-else-if-block
?>
// retrieve span object, and attach additional HTML to it
dynamic_span_obj          = document.getElementById( 'dynamic_span' );
dynamic_span_obj.innerHTML     += '<?php echo $inner_html; ?> <br>';
