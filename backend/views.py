from django.views.generic import View
from django.shortcuts import render, redirect
from backend.models import Device, Record
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.utils.decorators import method_decorator
import dateutil.parser
import json

class ProfileView(View):
    template_name = "account/profile.html"
    def get(self, request, *args, **kwargs):
        if request.user.is_anonymous: return redirect("/accounts/login")
        tokens = Device.objects.filter(user = request.user)
        return render(request, self.template_name)


class DeviceManager(View):

    def dispatch(self, *args, **kwargs):
        method = self.request.POST.get('_method', '').lower()
        if method == 'put':
            return self.put(*args, **kwargs)
        if method == 'delete':
            return self.delete(*args, **kwargs)
        return super(TestView, self).dispatch(*args, **kwargs)


    def put(self, request, *args, **kwargs):
        if request.user.is_anonymous:
            return redirect('/accounts/login')
        device = Device(user=request.user, name=request.POST['device_name'])
        device.save()
        print(device)
        return redirect('/accounts/profile')

    def delete(self, request, api_token):
        if request.user.is_anonymous:
            return redirect('/accounts/login')

        device = Device.objects.get(api_token = api_token)
        device.delete()

        return redirect('/accounts/profile')

@method_decorator(csrf_exempt, name='dispatch')
class Track(View):
    def post(self, request):
        body = json.loads(request.body)
        for item in body:
            device = None
            try:
                device = Device.objects.get(api_token=item["device"])
                moment = dateutil.parser.parse(item['moment'])
                rec = Record(device=device, reported_at=moment, application=item['application'], context = item['context'])
                rec.save()
            except BaseException as e:
                return HttpResponse(json.dumps({"status": "failure", "message": "Invalid device"})
                    , content_type="application/json")
        return HttpResponse(json.dumps({"status": "success"})
            , content_type="application/json")

@method_decorator(csrf_exempt, name='dispatch')
class DeviceLogin(View):
    def post(self, request):
        body = json.loads(request.body)
        device = body["device"]
        try:
            Device.objects.get(api_token=device)
        except:
            return HttpResponse(json.dumps({"status": "error", "message":"Unable to find specified device"}), content_type="application/json")
        return HttpResponse(json.dumps({"status": "success"}), content_type="application/json")

