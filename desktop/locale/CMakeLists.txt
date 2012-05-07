include( FindGettext OPTIONAL )

if( GETTEXT_FOUND )
  subdirs( libclaw )
endif( GETTEXT_FOUND )
