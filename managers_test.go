package lightningbug

import (
	"errors"
	"fmt"
	"os"
	"testing"

	"github.com/go-logr/logr"
	"github.com/go-logr/zapr"
	"github.com/go-logr/zerologr"
	"github.com/rs/zerolog"
	"go.uber.org/zap"
)

func testInternalLogging(lm LogManager) {
	lm.Logger.V(1).Info("Help")
	lm.Logger.Error(errors.New("fake err"), "testInternalLogging", "test", true)
}

func TestNewLogManager(t *testing.T) {
	zerolog.TimeFieldFormat = zerolog.TimeFormatUnixMs

	zerologr.NameFieldName = "logger"
	zerologr.NameSeparator = "/"
	zerologr.SetMaxV(2)

	zl := zerolog.New(os.Stderr)
	zl = zl.With().Caller().Timestamp().Logger()
	var log logr.Logger = zerologr.New(&zl)

	testInternalLogging(NewLogManager(log))

	zapLog, err := zap.NewDevelopment()
	if err != nil {
		panic(fmt.Sprintf("who watches the watchmen (%v)?", err))
	}
	log = zapr.NewLogger(zapLog)

	testInternalLogging(NewLogManager(log))
}

func TestGameManagerRun(t *testing.T) {

}
