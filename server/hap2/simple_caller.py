#!/usr/bin/env python
"""
  Copyright (C) 2015 Project Hatohol

  This file is part of Hatohol.

  Hatohol is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License, version 3
  as published by the Free Software Foundation.

  Hatohol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with Hatohol. If not, see
  <http://www.gnu.org/licenses/>.
"""

import haplib
import simple_server
import logging

class SimpleCaller:

    def __init__(self, transporter_args):
        self.__sender = haplib.Sender(transporter_args)
        self.__COMMAND_HANDLERS = {
            "exchangeProfile": self.__rpc_exchange_profile,
            "fetchTriggers": self.__rpc_fetch_triggers,
            "fetchEvents":   self.__rpc_fetch_events,
        }

    def __call__(self, args):
        logging.info("Command: %s" % args.command)
        self.__curr_command = args.command
        self.__curr_args = args
        self.__COMMAND_HANDLERS[args.command](args)
        self.__curr_command = None
        self.__curr_args = None

    def __rpc_exchange_profile(self, args):
        params = {"name": "SimpleCaller", "procedures": ["exchangeProfile"]}
        self.__request(params)

    def __rpc_fetch_triggers(self, args):
        params = {"hostIds": args.host_ids}
        self.__request(params)

    def __rpc_fetch_events(self, args):
        params = {"lastInfo": args.last_info, "count": args.count,
                  "direction": args.direction}
        self.__request(params)

    def __request(self, params):
        __component_code = 0
        request_id = haplib.Utils.generate_request_id(__component_code)
        params["fetchId"] = self.__curr_args.fetch_id
        self.__sender.request(self.__curr_command, params, request_id)

    @staticmethod
    def arg_def(parser):
        parser.add_argument('--fetch-id', default="1")
        subparsers = parser.add_subparsers(dest='command',
                                           help='sub-command help')

        parser_fetch_trig = subparsers.add_parser('exchangeProfile')

        parser_fetch_trig = subparsers.add_parser('fetchTriggers')
        parser_fetch_trig.add_argument('--host-ids', nargs="+", default=["1"])

        parser_fetch_evt = subparsers.add_parser('fetchEvents')
        parser_fetch_evt.add_argument('--last-info', default="")
        parser_fetch_evt.add_argument('--count', type=int, default=1000)
        parser_fetch_evt.add_argument('--direction', choices=["ASC", "DESC"],
                                      default="ASC")
        parser_fetch_evt.add_argument('--fetch-id', default="1")


if __name__ == '__main__':
    prog_name = "Simple Caller for HAPI 2.0"
    args, transporter_args = simple_server.basic_setup(SimpleCaller.arg_def,
                                                       prog_name=prog_name)
    caller = SimpleCaller(transporter_args)
    caller(args)
