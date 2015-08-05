# Bittrex Public API wrapper
import requests

class pubBittrex:
	def __init__(self):
		self.url = "https://bittrex.com/api/v1.1/public/getorderbook?market=BTC-"
		self.urlEnd = "&type=both&depth=50"

		self.coin = "limx"


		self.summeryURL = "https://bittrex.com/api/v1.1/public/getmarketsummary?market=btc-"

	def getSummery(self):
		
		data = {}

		path = self.summeryURL + self.coin

		newData = requests.get(path).json()

		print(newData)



b = pubBittrex()
b.getSummery()
