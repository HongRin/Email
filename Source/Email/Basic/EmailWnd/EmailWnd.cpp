#include "EmailWnd.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UEmailWnd::NativeConstruct()
{
	Super::NativeConstruct();

	EditableTextBox_Input->OnTextCommitted.AddDynamic(this, &UEmailWnd::OnEmailCommitted);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	// 허용되는 도메인을 추가합니다.
	LabelList.Add(FString(TEXT("naver.com")));
	LabelList.Add(FString(TEXT("daum.net")));
	LabelList.Add(FString(TEXT("google.com")));
	LabelList.Add(FString(TEXT("apple.com")));
	LabelList.Add(FString(TEXT("outlook.com")));

	// 레이블을 크기에 따라 정렬합니다.
	LabelList.Sort();

	LabelName.Add(FString(TEXT("naver.com")), FString(TEXT("네이버")));
	LabelName.Add(FString(TEXT("daum.net")), FString(TEXT("다음")));
	LabelName.Add(FString(TEXT("google.com")), FString(TEXT("구글")));
	LabelName.Add(FString(TEXT("apple.com")), FString(TEXT("애플")));
	LabelName.Add(FString(TEXT("outlook.com")), FString(TEXT("아웃룩")));

}

bool UEmailWnd::GetIsAt(FString str)
{
	int32 atCount = 0;
	for (int i = 0; i < str.Len(); ++i)
	{
		// @ 를 찾았다면
		if (str[i] == TCHAR(TEXT('@')))
			atCount++;
	}

	// @ 이 있다면
	if (atCount != 0)
	{
		// 인용부호안의 문자열을 가져옵니다.
		FString inQuotationMark = GetInQuotationMark(str);

		// str 과 inQuotationMark 이 같다면 트루를 반환합니다.
		if (str.Compare(inQuotationMark) == 0) return true;

		// 인용 부호안의 @ 의 개수를 체크합니다.
		int32 AtinQuotationMarkCount = 0;
		for (int i = 0; i < inQuotationMark.Len(); ++i)
		{
			if (inQuotationMark[i] == TCHAR(TEXT('@')))
				AtinQuotationMarkCount++;
		}

		if (atCount > AtinQuotationMarkCount) return true;
	}

	return false;
}

TTuple<FString, FString> UEmailWnd::GetIdAndDomainInAt(FString str)
{
	FString id;

	FString domain;

	// 마지막 @ 기준으로 문자열을 나눔

	FString read = TEXT("@");

	// 끝에서 부터 검사하여 @ 를 찾았다면 인덱스를 가져옵니다.
	int32 index = str.Find(read, ESearchCase::IgnoreCase, ESearchDir::FromEnd, str.Len());

	for (int i = 0; i < index; ++i)
	{
		id.AppendChar(str[i]);
	}

	for (int i = (index + 1); i < str.Len(); ++i)
	{
		domain.AppendChar(str[i]);
	}

	return MakeTuple(id, domain);
}

void UEmailWnd::OuputErrorText(EErrorType errorType)
{
	switch (errorType)
	{
	case EErrorType::ET_ATZERO:
		Text_Output->SetText(FText::FromString(TEXT("@ 를 찾을 수 없습니다.")));
		break;
	case EErrorType::ET_SPECIALSYMBOL:
		Text_Output->SetText(FText::FromString(TEXT("인용 부호밖에 특수 문자가 있습니다.")));
		break;
	case EErrorType::ET_DMSPECIALSYMBOL:
		Text_Output->SetText(FText::FromString(TEXT("도메인에 특수 문자가 있습니다.")));
		break;
	case EErrorType::ET_IDAPERIOD:
		Text_Output->SetText(FText::FromString(TEXT("로컬 파트에 . 이 두개 이상 있습니다.")));
		break;
	case EErrorType::ET_DOMAINAPERIOD:
		Text_Output->SetText(FText::FromString(TEXT("도메인에 . 이 두개 이상 있습니다.")));
		break;
	case EErrorType::ET_IDBLANK:
		Text_Output->SetText(FText::FromString(TEXT("이메일 주소의 시작 혹은 끝 지점에 공백이 있습니다.")));
		break;
	case EErrorType::ET_IDLENGTH:
		Text_Output->SetText(FText::FromString(TEXT("로컬 파트가 너무 길거나 존재하지 않습니다.")));
		break;
	case EErrorType::ET_DOMAINLENGTH:
		Text_Output->SetText(FText::FromString(TEXT("도메인이 너무 길거나 존재 하지 않습니다.")));
		break;
	case EErrorType::ET_DOMAINHYPHEN:
		Text_Output->SetText(FText::FromString(TEXT("도메인의 앞과 끝에는 - 를 사용하실 수 없습니다.")));
		break;
	case EErrorType::ET_NONELABEL:
		Text_Output->SetText(FText::FromString(TEXT("도메인에 레이블이 존재하지 않습니다.")));
		break;
	case EErrorType::ET_NOTFINDLABEL:
		Text_Output->SetText(FText::FromString(TEXT("해당 레이블이 존재하지 않습니다.")));
		break;
	}
}

EQuotationMarkType UEmailWnd::IsquotationMarkNumEven(FString str)
{
	int32 quotationMarkNum = 0;

	// " 의 개수를 구합니다.
	for (int32 i = 0; i < str.Len(); ++i)
	{
		if (str[i] == TCHAR(TEXT('\"')))
			++quotationMarkNum;
	}

	EQuotationMarkType type;

	if (quotationMarkNum == 0) type = EQuotationMarkType::QT_ZERO;
	else if (quotationMarkNum == 1) type = EQuotationMarkType::QT_ONE;
	else if ((quotationMarkNum % 2) == 0) type = EQuotationMarkType::QT_EVEN;
	else type = EQuotationMarkType::QT_ODD;

	return type;
}

FString UEmailWnd::GetInQuotationMark(FString str)
{
	if (IsquotationMarkNumEven(str) == EQuotationMarkType::QT_ONE) return FString(TEXT(""));
	if (IsquotationMarkNumEven(str) == EQuotationMarkType::QT_ZERO) return str;

	FString inQuotationMark;

	FString read = TEXT("\"");

	// 처음부터 검사하여 " 를 찾았다면 인덱스를 반환합니다.
	int32 startindex = str.Find(read, ESearchCase::IgnoreCase, ESearchDir::FromStart, 0);
	// 끝부터 검사하여 " 를 찾았다면 인덱스를 반환합니다.
	int32 endindex = str.Find(read, ESearchCase::IgnoreCase, ESearchDir::FromEnd, str.Len());

	// 인용부호 안의 문자열을 구합니다.
	for (int i = startindex; i < endindex + 1; ++i)
	{
		inQuotationMark.AppendChar(str[i]);
	}

	return inQuotationMark;
}

bool UEmailWnd::CheckIDSymbol(FString str)
{
	// 아스키 코드의 값에 따라 허용되는 범위를 지정합니다.
	for (int i = 0; i < str.Len(); ++i)
	{
		int32 iStr = str[i];

		if (iStr < 33) return false;
		else if (iStr > 57 && iStr < 61) return false;
		else if (iStr == 62) return false;
		else if (iStr > 90 && iStr < 94) return false;
		else if (iStr > 126) return false;
	}
	
	return true;
}

bool UEmailWnd::CheckDomainSymbol(FString str)
{
	for (int i = 0; i < str.Len(); ++i)
	{
		int32 iStr = str[i];

		if (iStr < 45) return false;
		else if (iStr > 46 && iStr < 48) return false;
		else if (iStr > 57 && iStr < 65) return false;
		else if (iStr > 90 && iStr < 97) return false;
		else if (iStr > 122) return false;
	}

	return true;
}

bool UEmailWnd::CheckAperiod(FString str)
{
	int32 aperiodNum = 0;

	// . 의 개수를 구합니다.
	for (int32 i = 0; i < str.Len(); ++i)
	{
		if (str[i] == TCHAR(TEXT('.')))
			++aperiodNum;
	}

	return aperiodNum < 2;
}

bool UEmailWnd::CheckBlank(FString str)
{
	// 이메일을 앞 뒤 사이에 공백이 있다면 true 를 반환합니다.
	return (str[0] == TCHAR(TEXT(' ')) || str[str.Len() - 1] == TCHAR(TEXT(' ')));
}

bool UEmailWnd::CheckLength(FString str)
{
	// 이메일이 없거나 너무 길다면 false 를 반환합니다.
	return (!str.IsEmpty() && str.Len() <= 63);
}

bool UEmailWnd::CheckHyphen(FString str)
{
	// 도메인의 처음과 끝에 - 이 있다면 true 를 반환합니다.
	return (str[0] == TCHAR(TEXT('-')) || str[str.Len() - 1] == TCHAR(TEXT('-')));
}

bool UEmailWnd::IsLabel(FString str)
{
	// 도메인의 처음에 . 이 있다면 true 를 반환합니다.
	return (str[0] == TCHAR(TEXT('.')));
}

bool UEmailWnd::IDCheck(FString checkID)
{
	if (CheckLength(checkID))
	{
		FString inQuotationMark = GetInQuotationMark(checkID);

		if (checkID.Compare(inQuotationMark) == 0)
		{
			if (CheckIDSymbol(checkID))
			{
				if (CheckAperiod(checkID)) return true;
				else OuputErrorText(EErrorType::ET_IDAPERIOD);
			}
			else OuputErrorText(EErrorType::ET_SPECIALSYMBOL);
		}
		else OuputErrorText(EErrorType::ET_SPECIALSYMBOL);
	}
	else OuputErrorText(EErrorType::ET_IDLENGTH);

	return false;
}

bool UEmailWnd::DomainCheck(FString checkDomain)
{
	if (CheckLength(checkDomain))
	{
		if (CheckDomainSymbol(checkDomain))
		{
			if (CheckAperiod(checkDomain))
			{
				if (!CheckHyphen(checkDomain))
				{
					if (!IsLabel(checkDomain)) return true;
					else OuputErrorText(EErrorType::ET_NONELABEL);
				}
				else OuputErrorText(EErrorType::ET_DOMAINHYPHEN);
			}
			else OuputErrorText(EErrorType::ET_DOMAINAPERIOD);
		}
		else OuputErrorText(EErrorType::ET_DMSPECIALSYMBOL);
	}
	else OuputErrorText(EErrorType::ET_DOMAINLENGTH);

	return false;
}

bool UEmailWnd::LabelCheck(FString checkLabel)
{
	int32 start = 0;
	int32 end = LabelList.Num() - 1;
	int32 mid;

	// 이진 탐색으로 유효한 레이블을 찾습니다.
	while (start <= end) {
		mid = (start + end) / 2;

		int32 compareResult = checkLabel.Compare(LabelList[mid]);
		if (compareResult == 0) return true;
		else if (compareResult == -1) end = mid - 1;
		else start = mid + 1;
	}

	return false;
}

FString UEmailWnd::GetLabel(FString label)
{
	// 레이블의 이름을 가져옵니다.
	return *LabelName.Find(label);
}

void UEmailWnd::OnEmailCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 엔터키를 눌렀다면
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// 입력한 email 을 FString 으로 담을 변수
		FString emailStr = Text.ToString();

		FString id;

		FString domain;

		if (GetIsAt(emailStr))
		{
			if (!CheckBlank(emailStr))
			{
				auto tupleData = GetIdAndDomainInAt(emailStr);
				id = tupleData.Get<0>();
				domain = tupleData.Get<1>();

				if (IDCheck(id) && DomainCheck(domain))
				{	
					if (LabelCheck(domain))
					{
						Text_Output->SetText(FText::FromString(TEXT("ID[") + id + TEXT("]::Domain[") + GetLabel(domain) + TEXT("]")));
					}
					else OuputErrorText(EErrorType::ET_NOTFINDLABEL);
				}
			}
			else  OuputErrorText(EErrorType::ET_IDBLANK);
		}
		else OuputErrorText(EErrorType::ET_ATZERO);
	}
}