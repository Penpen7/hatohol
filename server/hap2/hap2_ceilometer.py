#!/usr/bin/env python
# coding: UTF-8
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

import sys
import MySQLdb
import time
import haplib
import standardhap
import logging

class Common:

    def __init__(self):
        pass

    def close_connection(self):
        # remove session ID
        pass

    def ensure_connection(self):
        # get the session ID
        pass

    def collect_hosts_and_put(self):
        pass

    def collect_host_groups_and_put(self):
        pass

    def collect_host_group_membership_and_put(self):
        pass

    def collect_triggers_and_put(self, fetch_id=None, host_ids=None):
        pass

    def collect_events_and_put(self, fetch_id=None, last_info=None,
                               count=None, direction="ASC"):
        pass


class Hap2CeilometerPoller(haplib.BasePoller, Common):

    def __init__(self, *args, **kwargs):
        haplib.BasePoller.__init__(self, *args, **kwargs)
        Common.__init__(self)

    def poll_setup(self):
        self.ensure_connection()

    def poll_hosts(self):
        self.collect_hosts_and_put()

    def poll_host_groups(self):
        self.collect_host_groups_and_put()

    def poll_host_group_membership(self):
        self.collect_host_group_membership_and_put()

    def poll_triggers(self):
        self.collect_triggers_and_put()

    def poll_events(self):
        self.collect_events_and_put()

    def on_aborted_poll(self):
        self.reset()
        self.close_connection()


class Hap2CeilometerMain(haplib.BaseMainPlugin, Common):
    def __init__(self, *args, **kwargs):
        haplib.BaseMainPlugin.__init__(self, kwargs["transporter_args"])
        Common.__init__(self)

    def hap_fetch_triggers(self, params, request_id):
        self.ensure_connection()
        fetch_id = params["fetchId"]
        host_ids = params["hostIds"]
        self.collect_triggers_and_put(fetch_id, host_ids)

    def hap_fetch_events(self, params, request_id):
        self.ensure_connection()
        self.collect_events_and_put(params["fetchId"], params["lastInfo"],
                                    params["count"], params["direction"])

    def hap_fetch_items(self, params, request_id):
        logging.error("Not implemented")

    def hap_fetch_items(self, params, request_id):
        logging.error("Not implemented")


class Hap2Ceilometer(standardhap.StandardHap):
    def create_main_plugin(self, *args, **kwargs):
        return Hap2CeilometerMain(*args, **kwargs)

    def create_poller(self, *args, **kwargs):
        return Hap2CeilometerPoller(self, *args, **kwargs)


if __name__ == '__main__':
    hap = Hap2Ceilometer()
    hap()
