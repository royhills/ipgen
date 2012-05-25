/*
 * ipgen is Copyright (C) 2012 Roy Hills, NTA Monitor Ltd.
 *
 * This file is part of ipgen.
 * 
 * ipgen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ipgen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ipgen  If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 *
 * ipgen
 *
 * Author:	Roy Hills
 * Date:	22 May 2012
 *
 * Usage:
 *    ipgen [options] <IP-network>
 *
 * Description:
 *
 * ipgen generates a list of IP host addresses from an IP network specification.
 *
 */

#include "ipgen.h"

static char const rcsid[] = "$Id$";   /* RCS ID for ident(1) */

/* Global variables */
static char filename[MAXLINE];
static int filename_flag=0;

int
main(int argc, char *argv[]) {
/*
 *      Process options.
 */
   process_options(argc, argv);
/*
 *	Process the networks from the specified file if --file was specified,
 *	or otherwise from the remaining command line arguments.
 */
   if (filename_flag) { /* Populate list from file */
      FILE *fp;
      char line[MAXLINE];
      char *cp;

      if ((strcmp(filename, "-")) == 0) {       /* Filename "-" means stdin */
         fp = stdin;
      } else {
         if ((fp = fopen(filename, "r")) == NULL) {
            err_sys("fopen");
         }
      }

      while (fgets(line, MAXLINE, fp)) {
         for (cp = line; !isspace((unsigned char)*cp) && *cp != '\0'; cp++)
            ;
         *cp = '\0';
         process_network(line);
      }
      if (fp != stdin) {
         fclose(fp);
      }
   } else {             /* Populate list from command line arguments */
      argv=&argv[optind];
      while (*argv) {
         process_network(*argv);
         argv++;
      }
   }


   return 0;
}

/*
 *	usage -- display usage message and exit
 *
 *	Inputs:
 *
 *	status		Status code to pass to exit()
 *	detailed	zero for brief output, non-zero for detailed output
 *
 *	Returns:
 *
 *	None (this function never returns).
 */
void
usage(int status, int detailed) {
   fprintf(stderr, "Usage: ipgen [options] <IP-network>\n");
   fprintf(stderr, "\n");
   fprintf(stderr, "The IP network can be specified as\n");
   fprintf(stderr, "IPnetwork/bits (e.g. 192.168.1.0/24) to specify all hosts\n");
   fprintf(stderr, "in the given network (network and broadcast addresses excluded) or\n");
   fprintf(stderr, "IPstart-IPend (e.g. 192.168.1.3-192.168.1.27) to specify all hosts in the\n");
   fprintf(stderr, "inclusive range, or IPnetwork:NetMask (e.g. 192.168.1.0:255.255.255.0) to\n");
   fprintf(stderr, "specify all hosts in the given network and mask.\n");
   fprintf(stderr, "\n");
   fprintf(stderr, "These different options for specifying target hosts may be used both on the\n");
   fprintf(stderr, "command line, and also in the file specified with the --file option.\n");
   fprintf(stderr, "\n");
   if (detailed) {
      fprintf(stderr, "Options:\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "\n--help or -h\t\tDisplay this usage message and exit.\n");
      fprintf(stderr, "\n--file=<s> or -f <s>\tRead addresses from the specified file\n");
      fprintf(stderr, "\t\t\tinstead of from the command line. One IP\n");
      fprintf(stderr, "\t\t\taddress per line. Use \"-\" for standard input.\n");
      fprintf(stderr, "\n--version or -V\t\tDisplay program version and exit.\n");
   } else {
      fprintf(stderr, "use \"ipgen --help\" for detailed information on the available options.\n");
   }
   fprintf(stderr, "\n");
   fprintf(stderr, "Report bugs or send suggestions to %s\n", PACKAGE_BUGREPORT);
   fprintf(stderr, "See the ipgen homepage at http://www.nta-monitor.com/tools/ipgen/\n");
   exit(status);
}

/*
 *      process_network -- Process an IP network specification.
 *
 *      Inputs:
 *
 *      pattern = The IP network pattern to process.
 *
 *      Returns: None
 *
 *      This processes the specified IP network. The pattern argument
 *      can either be a IP address, in which case one host
 *      will be processed to the list, or it can specify a number of hosts
 *	with the IPnet/bits or IPstart-IPend formats.
 */
void
process_network(const char *pattern) {
   char *patcopy;
   struct in_addr in_val;
   struct in_addr mask_val;
   unsigned numbits;
   char *cp;
   uint32_t ipnet_val;
   uint32_t network;
   uint32_t mask;
   unsigned long hoststart;
   unsigned long hostend;
   unsigned i;
   uint32_t x;
   static int first_call=1;
   static regex_t iprange_pat;
   static regex_t ipslash_pat;
   static regex_t ipmask_pat;
   static const char *iprange_pat_str =
      "[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+-[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+";
   static const char *ipslash_pat_str =
      "[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+/[0-9]+";
   static const char *ipmask_pat_str =
      "[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+:[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+";
/*
 *	Compile regex patterns if this is the first time we've been called.
 */
   if (first_call) {
      int result;

      first_call = 0;
      if ((result=regcomp(&iprange_pat, iprange_pat_str,
                          REG_EXTENDED|REG_NOSUB))) {
         char errbuf[MAXLINE];
         size_t errlen;
         errlen=regerror(result, &iprange_pat, errbuf, MAXLINE);
         err_msg("ERROR: cannot compile regex pattern \"%s\": %s",
                 iprange_pat_str, errbuf);
      }
      if ((result=regcomp(&ipslash_pat, ipslash_pat_str,
                          REG_EXTENDED|REG_NOSUB))) {
         char errbuf[MAXLINE];
         size_t errlen;
         errlen=regerror(result, &ipslash_pat, errbuf, MAXLINE);
         err_msg("ERROR: cannot compile regex pattern \"%s\": %s",
                 ipslash_pat_str, errbuf);
      }
      if ((result=regcomp(&ipmask_pat, ipmask_pat_str,
                          REG_EXTENDED|REG_NOSUB))) {
         char errbuf[MAXLINE];
         size_t errlen;
         errlen=regerror(result, &ipmask_pat, errbuf, MAXLINE);
         err_msg("ERROR: cannot compile regex pattern \"%s\": %s",
                 ipmask_pat_str, errbuf);
      }
   }
/*
 *	Make a copy of pattern because we don't want to modify our argument.
 */
   patcopy = dupstr(pattern);

   if (!(regexec(&ipslash_pat, patcopy, 0, NULL, 0))) { /* IPnet/bits */
/*
 *	Get IPnet and bits as integers. Perform basic error checking.
 */
      cp=strchr(patcopy, '/');
      *(cp++)='\0';	/* patcopy points to IPnet, cp points to bits */
      if (!(inet_aton(patcopy, &in_val)))
         err_msg("ERROR: %s is not a valid IP address", patcopy);
      ipnet_val=ntohl(in_val.s_addr);	/* We need host byte order */
      numbits=Strtoul(cp, 10);
      if (numbits<3 || numbits>32)
         err_msg("ERROR: Number of bits in %s must be between 3 and 32",
                 pattern);
/*
 *	Construct 32-bit network bitmask from number of bits.
 */
      mask=0;
      for (i=0; i<numbits; i++)
         mask += 1 << i;
      mask = mask << (32-i);
/*
 *	Mask off the network. Warn if the host bits were non-zero.
 */
      network=ipnet_val & mask;
      if (network != ipnet_val)
         warn_msg("WARNING: host part of %s is non-zero", pattern);
/*
 *	Determine maximum and minimum host values. We include the host
 *	and broadcast.
 */
      hoststart=0;
      hostend=(1<<(32-numbits))-1;
/*
 *	Calculate all host addresses in the range and print in dotted-quad
 *	format.
 */
      for (i=hoststart; i<=hostend; i++) {
         uint32_t hostip;
         int b1, b2, b3, b4;

         hostip = network+i;
         b1 = (hostip & 0xff000000) >> 24;
         b2 = (hostip & 0x00ff0000) >> 16;
         b3 = (hostip & 0x0000ff00) >> 8;
         b4 = (hostip & 0x000000ff);
         printf("%d.%d.%d.%d\n", b1,b2,b3,b4);
      }
   } else if (!(regexec(&ipmask_pat, patcopy, 0, NULL, 0))) { /* IPnet:netmask */
/*
 *	Get IPnet and bits as integers. Perform basic error checking.
 */
      cp=strchr(patcopy, ':');
      *(cp++)='\0';	/* patcopy points to IPnet, cp points to netmask */
      if (!(inet_aton(patcopy, &in_val)))
         err_msg("ERROR: %s is not a valid IP address", patcopy);
      ipnet_val=ntohl(in_val.s_addr);	/* We need host byte order */
      if (!(inet_aton(cp, &mask_val)))
         err_msg("ERROR: %s is not a valid netmask", patcopy);
      mask=ntohl(mask_val.s_addr);	/* We need host byte order */
/*
 *	Calculate the number of bits in the network.
 */
      x = mask;
      for (numbits=0; x != 0; x>>=1) {
         if (x & 0x01) {
            numbits++;
         }
      }
/*
 *	Mask off the network. Warn if the host bits were non-zero.
 */
      network=ipnet_val & mask;
      if (network != ipnet_val)
         warn_msg("WARNING: host part of %s is non-zero", pattern);
/*
 *	Determine maximum and minimum host values. We include the host
 *	and broadcast.
 */
      hoststart=0;
      hostend=(1<<(32-numbits))-1;
/*
 *	Calculate all host addresses in the range and print in dotted-quad
 *	format.
 */
      for (i=hoststart; i<=hostend; i++) {
         uint32_t hostip;
         int b1, b2, b3, b4;

         hostip = network+i;
         b1 = (hostip & 0xff000000) >> 24;
         b2 = (hostip & 0x00ff0000) >> 16;
         b3 = (hostip & 0x0000ff00) >> 8;
         b4 = (hostip & 0x000000ff);
         printf("%d.%d.%d.%d\n", b1,b2,b3,b4);
      }
   } else if (!(regexec(&iprange_pat, patcopy, 0, NULL, 0))) { /* IPstart-IPend */
/*
 *	Get IPstart and IPend as integers.
 */
      cp=strchr(patcopy, '-');
      *(cp++)='\0';	/* patcopy points to IPstart, cp points to IPend */
      if (!(inet_aton(patcopy, &in_val)))
         err_msg("ERROR: %s is not a valid IP address", patcopy);
      hoststart=ntohl(in_val.s_addr);	/* We need host byte order */
      if (!(inet_aton(cp, &in_val)))
         err_msg("ERROR: %s is not a valid IP address", cp);
      hostend=ntohl(in_val.s_addr);	/* We need host byte order */
/*
 *	Calculate all host addresses in the range and print in dotted-quad
 *	format.
 */
      for (i=hoststart; i<=hostend; i++) {
         int b1, b2, b3, b4;

         b1 = (i & 0xff000000) >> 24;
         b2 = (i & 0x00ff0000) >> 16;
         b3 = (i & 0x0000ff00) >> 8;
         b4 = (i & 0x000000ff);
         printf("%d.%d.%d.%d\n", b1,b2,b3,b4);
      }
   } else {	/* Single host or IP address */
      printf("%s\n", patcopy);
   }
   free(patcopy);
}

/*
 *	process_options	--	Process options and arguments.
 *
 *	Inputs:
 *
 *	argc	Command line arg count
 *	argv	Command line args
 *
 *	Returns:
 *
 *	None.
 */
void
process_options(int argc, char *argv[]) {
   struct option long_options[] = {
      {"file", required_argument, 0, 'f'},
      {"help", no_argument, 0, 'h'},
      {"version", no_argument, 0, 'V'},
      {0, 0, 0, 0}
   };
   const char *short_options =
      "f:hV";
   int arg;
   int options_index=0;

   while ((arg=getopt_long_only(argc, argv, short_options, long_options, &options_index)) != -1) {
      switch (arg) {
         case 'f':	/* --file */
            strlcpy(filename, optarg, sizeof(filename));
            filename_flag=1;
            break;
         case 'h':	/* --help */
            usage(EXIT_SUCCESS, 1);
            break;	/* NOTREACHED */
         case 'V':	/* --version */
            ipgen_version();
            exit(0);
            break;	/* NOTREACHED */
         default:	/* Unknown option */
            usage(EXIT_FAILURE, 0);
            break;	/* NOTREACHED */
      }
   }
}

/*
 *	ipgen_version -- display version information
 *
 *	Inputs:
 *
 *	None.
 *
 *	Returns:
 *
 *	None.
 *
 *	This displays the ipgen version information.
 */
void
ipgen_version (void) {
   fprintf(stderr, "%s\n\n", PACKAGE_STRING);
   fprintf(stderr, "Copyright (C) 2005-2011 Roy Hills, NTA Monitor Ltd.\n");
   fprintf(stderr, "ipgen comes with NO WARRANTY to the extent permitted by law.\n");
   fprintf(stderr, "You may redistribute copies of arp-scan under the terms of the GNU\n");
   fprintf(stderr, "General Public License.\n");
   fprintf(stderr, "For more information about these matters, see the file named COPYING.\n");
   fprintf(stderr, "\n");
/* We use rcsid here to prevent it being optimised away */
   fprintf(stderr, "%s\n", rcsid);
   error_use_rcsid();
   wrappers_use_rcsid();
   utils_use_rcsid();
}
