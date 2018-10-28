from django.db.models import *
from django.contrib.auth.models import User
from uuid import uuid4

class Device(Model):
    user = ForeignKey(User, on_delete=CASCADE, related_name='devices')
    api_token = CharField(max_length=512, default=uuid4)
    name = CharField(max_length=512)

    def len(self):
        return len( self.records.all() )

class Record(Model):
    # This is a DEVICE which user was operating with
    device = ForeignKey(Device, on_delete=CASCADE, related_name='records')

    # When this was reported. It should be CLIENT time, not SERVER
    reported_at = DateTimeField()

    # Application which was under user's eyes (a.k.a. "Active Window")
    application = CharField(max_length=512)

    # Context. Most propably window title.
    context = CharField(max_length=4096)

